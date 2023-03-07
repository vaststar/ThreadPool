#include "ThreadPool.h"

#include <vector>
#include <string>
#include <iostream>
#include <functional>

void test_future_api()
{
	auto testFunc = [](int i)->int{std::this_thread::sleep_for(std::chrono::milliseconds(100)); return i*i;};
	ThreadPool pool(2,"future-mode");
	//test future mode
	std::vector<std::future<int>> test;
	for (int i = 0; i < 20; ++i) {
		test.push_back(pool.enqueueFutureFunc("testFutureFunc-"+std::to_string(i),ThreadPool::ThreadLevel::Level_Normal, testFunc,i));
	}
	for (int i = 0; i < 10; ++i) {
		test.push_back(pool.enqueueFutureFunc("urgent_testFutureFunc-"+std::to_string(i),ThreadPool::ThreadLevel::Level_Urgent, testFunc,i));
	}
	for(auto &tt : test)
	{
		std::cout<<tt.get()<<std::endl;
	}
}

void test_normal_api()
{
	auto testFunc = [](int i){std::this_thread::sleep_for(std::chrono::milliseconds(100)); std::cout << i*i<<std::endl; };
	ThreadPool pool(2,"mormal-mode");
	//test future mode
	std::vector<std::future<int>> test;
	for (int i = 0; i < 20; ++i) {
		pool.enqueueFunc("testNormalFunc-"+std::to_string(i),ThreadPool::ThreadLevel::Level_Normal, std::bind(testFunc,i));
	}
	for (int i = 0; i < 10; ++i) {
		pool.enqueueFunc("urgent_testNormalFunc-"+std::to_string(i),ThreadPool::ThreadLevel::Level_Urgent, std::bind(testFunc,i));
	}
}

int main(int argc, char* argv[])
{
	if(argc >= 2)
	{
		if(std::string(argv[1]) == "future")
		{
			test_future_api();
		}
		else if(std::string(argv[1]) == "normal")
		{
			test_normal_api();
		}
	}
	return 0;
}