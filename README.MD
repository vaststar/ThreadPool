# Thread Pool using C++
A very nice thread pool in c++11 written by Thomas Zhu.

# Usage
* firstly, link library in your project-set
* write some common code in your codings

## Using in project
* install first
* set dir in your CMakeList,eg `set(ThreadPool_DIR $logInstalledDir$/ThreadPool/install/Release/cmake) `
* use find package , `find_package(ThreadPool REQUIRED)`
* include headers, `include_directories(${ThreadPool_INCLUDE_DIRS})`
* link target to you library, `target_link_libraries(testProgram ThreadPoolExport::ThreadPool)`

## Using in code
* `#include "ThreadPool.h"`
* create threadpool object, `ThreadPool pool(4,"testpool");`
* push your function, `pool.enqueueFunc("test func",1,std::bind(functionname,params));`
* Or get future function `auto temp = pool.enqueueFutureFunc("test func",1,functionname,params); auto result = temp.get();`

# Requirement
* >= c++11
* >= cmake3.1

# Install
* make and cd in `build` dir
* run commond: `cmake .. && make && make install`

