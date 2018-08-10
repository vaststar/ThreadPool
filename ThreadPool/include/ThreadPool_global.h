#ifndef ThreadPool_global_h__
#define ThreadPool_global_h__

#ifdef THREADPOOL_LIB
#define THREADPOOL_EXPORT _declspec(dllexport)
#else
#define THREADPOOL_EXPORT _declspec(dllimport)
#endif
#endif // ThreadPool_global_h__