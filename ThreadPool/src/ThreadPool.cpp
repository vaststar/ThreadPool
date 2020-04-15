#include "ThreadPool.h"

#include <vector>
#include <list>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <iostream>
#include <ostream>
#include <sstream>

static void OutPut(std::string messages){std::ostringstream _oss; _oss << messages;std::cout<<_oss.str()<<std::endl;};
class ThreadPoolTask
{
public:
	ThreadPoolTask(){};
	ThreadPoolTask(uint32_t taskLevel, std::string taskTag, std::function<void()> functionTask)
		:m_taskLevel(taskLevel),m_taskTag(std::move(taskTag)),m_functionTask(std::move(functionTask))
	{

	}
	void execute()
	{
		OutPut("will execute function:["+m_taskTag+"]");
		m_functionTask();
	}
	bool operator<(const ThreadPoolTask &rls)
	{
		return m_taskLevel < rls.m_taskLevel;
	}
	bool operator<(uint32_t level)
	{
		return m_taskLevel < level;
	}
	bool operator>=(uint32_t level)
	{
		return m_taskLevel >= level;
	}
private:
	uint32_t m_taskLevel;
	std::string m_taskTag;
	std::function<void()> m_functionTask;
};
class ThreadPool::DataPrivate
{
public:
	explicit DataPrivate(std::string poolName)
		:stop(false)
		,threadPoolName(std::move(poolName))
	{

	}
	~DataPrivate()
	{
	}
public:
	std::vector< std::thread > workers;
	std::list<ThreadPoolTask> tasks;
	std::mutex tasks_mutex;
	std::condition_variable condition;
	bool stop;
	std::string threadPoolName;
};

ThreadPool::ThreadPool(uint32_t maxpool, std::string poolName )
	:_p(std::make_shared<DataPrivate>(std::move(poolName)))
{
	initPool(maxpool);
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

void ThreadPool::initPool(uint32_t poolNumber)
{
	for (uint32_t i = 0; i < poolNumber; ++i){
		_p->workers.emplace_back(
			[this]() {
				while (true){
					ThreadPoolTask task;
					{
						std::unique_lock<std::mutex> lock(this->_p->tasks_mutex);
						this->_p->condition.wait(lock,[this] { return this->_p->stop || !this->_p->tasks.empty(); });
						if (this->_p->stop && this->_p->tasks.empty()) return;
						
						// 取得任务队首任务,移动构造
						task = std::move(this->_p->tasks.front());
						// 从队列移除
						this->_p->tasks.pop_front();
					}
					// 执行任务
					task.execute();
				}
			}
		);
	}
}

void ThreadPool::enqueueFunc(std::string functionTag, uint32_t urgentLevel, std::function<void()> &&task)
{
	pushFuncPri(functionTag,std::move(urgentLevel),std::move(task));
}

void ThreadPool::pushFuncPri(std::string functionTag, uint32_t urgentLevel, std::function<void()> &&task)
{
	{
		std::unique_lock<std::mutex> lock(_p->tasks_mutex);
		if (_p->stop) return;
		OutPut(_p->threadPoolName + " enqueue task:["+functionTag+"]");
		// 将任务添加到任务队列
		for(auto it = _p->tasks.begin(); it != _p->tasks.end(); ++it)
		{
			if(*it >= urgentLevel)
			{
				_p->tasks.push_back(ThreadPoolTask(urgentLevel,std::move(functionTag),task));
				break;
			}
		}
		if(_p->tasks.empty())
		{
			_p->tasks.push_back(ThreadPoolTask(urgentLevel,std::move(functionTag),task));
		}
	}
	_p->condition.notify_one();
}
