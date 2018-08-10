#ifndef ThreadPool_h__
#define ThreadPool_h__

#include <stdint.h>
#include <future>
#include <functional>
#include "ThreadPool_global.h"

class THREADPOOL_EXPORT ThreadPool
{
public:
	explicit ThreadPool(uint32_t maxpool = 5000);
	~ThreadPool();
public:
	template<typename Fun, typename... Args>
	auto PushFunc(Fun &&f, Args&&... args)->std::future<typename std::result_of<Fun(Args...)>::type>;
private:
	void InitPool(uint32_t poolNumber);
	void PushFuncPri(std::function<void()>&&);
private:
	class DataPrivate;
	DataPrivate *_p;
};

template<typename Fun, typename... Args>
auto ThreadPool::PushFunc(Fun &&f, Args&&... args) ->std::future<typename std::result_of<Fun(Args...)>::type>
{
	using return_type = typename std::result_of<Fun(Args...)>::type;

	// 将任务函数和其参数绑定，构建一个packaged_task
	auto task = std::make_shared< std::packaged_task<return_type()> >(
		std::bind(std::forward<Fun>(f), std::forward<Args>(args)...)
		);
	// 获取任务的future
	std::future<return_type> res = task->get_future();
	PushFuncPri([task] {(*task)(); });
	return res;
}

#endif // ThreadPool_h__