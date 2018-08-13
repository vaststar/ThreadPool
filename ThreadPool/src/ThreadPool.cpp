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
		std::unique_lock<std::mutex> lock(_p->tasks_mutex);
		_p->stop = true;
	}
	_p->condition.notify_all();
	for (std::thread &worker : _p->workers) {
		worker.join();
	}
	delete _p;
	_p = nullptr;
}

void ThreadPool::InitPool(uint32_t poolNumber)
{
	for (uint32_t i = 0; i < poolNumber; ++i){
		_p->workers.emplace_back(
			[this]() {
				while (true){
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(this->_p->tasks_mutex);
						// 如果当前是停止状态或者任务为空，则等待
						this->_p->condition.wait(lock,[this] { return this->_p->stop || !this->_p->tasks.empty(); });
						// 如果线程池停止或者任务队列为空，结束返回
						if (this->_p->stop && this->_p->tasks.empty()) return;
						
						// 取得任务队首任务,移动构造
						task = std::move(this->_p->tasks.front());
						// 从队列移除
						this->_p->tasks.pop();
					}
					// 执行任务
					task();
				}
			}
		);

	}
}

void ThreadPool::PushFuncPri(std::function<void()> &&task)
{
	{
		std::unique_lock<std::mutex> lock(_p->tasks_mutex);
		// 不允许入队到已经停止的线程池
		if (_p->stop) {
			throw std::runtime_error("enqueue on stopped ThreadPool");
		}
		// 将任务添加到任务队列
		_p->tasks.emplace(task);
	}
	_p->condition.notify_one();
}
