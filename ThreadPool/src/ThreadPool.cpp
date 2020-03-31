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
	:_p(std::make_shared<DataPrivate>())
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
						this->_p->condition.wait(lock,[this] { return this->_p->stop || !this->_p->tasks.empty(); });
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
		if (_p->stop) return;
		// 将任务添加到任务队列
		_p->tasks.emplace(task);
	}
	_p->condition.notify_one();
}
