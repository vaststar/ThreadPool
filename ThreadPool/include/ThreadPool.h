#ifndef ThreadPool_h__
#define ThreadPool_h__

#include <memory>
#include <stdint.h>
#include <future>
#include <functional>
#include "ThreadPool_global.h"

class THREADPOOL_EXPORT ThreadPool
{
public:
	enum class ThreadLevel:uint32_t{
		Level_Urgent = 0,
		Level_High,
		Level_Normal,
		Level_Low
	};
public:
	ThreadPool(uint32_t maxpool = 5000, std::string poolName = "default-thread-pool");
	ThreadPool(ThreadPool &&) = delete;
	ThreadPool(const ThreadPool &) = delete;
	ThreadPool& operator=(const ThreadPool &) = delete;
	ThreadPool& operator=(ThreadPool &&) = delete;
	~ThreadPool();
public:
	/*enqueue fuction and get std::future result to wait*/
	template<typename Fun, typename... Args>
	auto enqueueFutureFunc(std::string functionTag, ThreadLevel urgentLevel, Fun &&f, Args&&... args)->std::future<typename std::result_of<Fun(Args...)>::type>;
	/*just enqueue function*/
	void enqueueFunc(std::string functionTag, ThreadLevel urgentLevel, std::function<void()> func);
private:
	void initPool(uint32_t poolNumber);
	void pushFuncPri(std::string functionTag, ThreadLevel urgentLevel, std::function<void()>&&);
private:
	class DataPrivate;
	std::shared_ptr<DataPrivate> _p;
};

template<typename Fun, typename... Args>
auto ThreadPool::enqueueFutureFunc(std::string functionTag, ThreadLevel urgentLevel, Fun &&f, Args&&... args) ->std::future<typename std::result_of<Fun(Args...)>::type>
{
	using return_type = typename std::result_of<Fun(Args...)>::type;

	// 将任务函数和其参数绑定，构建一个packaged_task
	auto task = std::make_shared< std::packaged_task<return_type()> >(
		std::bind(std::forward<Fun>(f), std::forward<Args>(args)...)
		);
	// 获取任务的future
	std::future<return_type> res = task->get_future();
	pushFuncPri(std::move(functionTag), urgentLevel, [task] {(*task)();});
	return res;
}
#endif // ThreadPool_h__