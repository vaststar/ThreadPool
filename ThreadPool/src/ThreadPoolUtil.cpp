#include "ThreadPoolUtil.h"
#include "ThreadPoolControl.h"

namespace ThreadPoolUtil
{

void initThreadPool(uint32_t maxThreadNumber, const std::string& threadPoolName)
{
    ThreadPoolControl::getInstance()->createThreadPool(maxThreadNumber, threadPoolName);
}

void createThreadTask(std::function<void()> func, ThreadLevel urgentLevel, const std::string& poolName, const std::string& functionTag)
{
    ThreadPoolControl::getInstance()->createTask(std::move(func), functionTag, static_cast<uint32_t>(urgentLevel), poolName);
}
}