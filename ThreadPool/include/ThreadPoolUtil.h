#ifndef ThreadPool_global_h__
#define ThreadPool_global_h__

#include <string>
#include <functional>
#include "ThreadPoolExport.h"

namespace ThreadPoolUtil
{
	enum class ThreadLevel:uint32_t{
		Level_Urgent = 0,
		Level_High,
		Level_Normal,
		Level_Low
	};
    static const std::string DefaultThreadPoolName = "default-thread-pool";
    static const std::string DefaultFunctionTag = "default-function-tag";
    THREADPOOL_EXPORT void initThreadPool(uint32_t maxThreadNumber = 500, const std::string& threadPoolName = DefaultThreadPoolName);
    THREADPOOL_EXPORT void createThreadTask(std::function<void()> func, ThreadLevel urgentLevel = ThreadLevel::Level_Normal, const std::string& poolName = DefaultThreadPoolName, const std::string& functionTag = DefaultFunctionTag);
}
#endif // ThreadPool_global_h__