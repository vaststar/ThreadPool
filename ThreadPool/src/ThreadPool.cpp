#include "ThreadPool.h"

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

class ThreadPool::DataPrivate
{
public:
	DataPrivate()
		:stop(false)
	{

	}
	~DataPrivate()
	{
	}
public:
	std::vector< std::thread > workers;
	std::queue<std::function<void()>> tasks;
	std::mutex tasks_mutex;
	std::condition_variable condition;
	bool stop;
};

ThreadPool::ThreadPool(uint32_t maxpool)
	:_p(new DataPrivate())
{
	InitPool(maxpool);
}

ThreadPool::~ThreadPool()
{
	{
		// ����
		std::unique_lock<std::mutex> lock(_p->tasks_mutex);
		// ֹͣ��־��true
		_p->stop = true;
	}
	// ֪ͨ���й����̣߳����Ѻ���ΪstopΪtrue�ˣ����Զ������
	_p->condition.notify_all();
	// �ȴ����й����߳̽���
	for (std::thread &worker : _p->workers) {
		worker.join();
	}
	delete _p;
	_p = nullptr;
}

void ThreadPool::InitPool(uint32_t poolNumber)
{
	for (uint32_t i = 0; i < poolNumber; ++i)
	{
		_p->workers.emplace_back(
			[this]() {
				while (true)
				{
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(this->_p->tasks_mutex);
						// �����ǰ��ֹͣ״̬��������Ϊ�գ���ȴ�
						this->_p->condition.wait(lock,[this] { return this->_p->stop || !this->_p->tasks.empty(); });
						// ����̳߳�ֹͣ�����������Ϊ�գ���������
						if (this->_p->stop && this->_p->tasks.empty()) return;
						
						// ȡ�������������,�ƶ�����
						task = std::move(this->_p->tasks.front());
						// �Ӷ����Ƴ�
						this->_p->tasks.pop();
					}
					// ִ������
					task();
				}
			}
		);

	}
}

void ThreadPool::PushFuncPri(std::function<void()> &&task)
{
	{
		// ��ռ����
		std::unique_lock<std::mutex> lock(_p->tasks_mutex);

		// don't allow enqueueing after stopping the pool
		// ��������ӵ��Ѿ�ֹͣ���̳߳�
		if (_p->stop) {
			throw std::runtime_error("enqueue on stopped ThreadPool");
		}
		// ��������ӵ��������
		_p->tasks.emplace(task);
	}
	_p->condition.notify_one();
}
