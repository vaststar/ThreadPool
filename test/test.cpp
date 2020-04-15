#include "ThreadPool.h"

#include <string>
#include <iostream>
#include <functional>

void test_future_api()
{
	auto testFunc = [](int i)->int{std::this_thread::sleep_for(std::chrono::seconds(1)); return i*i;};
	ThreadPool pool(1,"future-mode");
	//test future mode
	std::vector<std::future<int>> test;
	for (int i = 0; i < 20; ++i) {
		test.push_back(pool.enqueueFutureFunc("testFutureFunc-"+std::to_string(i),0, testFunc,i));
	}
	for(auto &tt : test)
	{
		std::cout<<tt.get()<<std::endl;
	}
}
void testF() {  }
void test_normal_api()
{
	std::function<void()> testFunc = [](){std::this_thread::sleep_for(std::chrono::seconds(1)); std::cout << 1; };
	ThreadPool pool(1,"mormal-mode");
	//test future mode
	std::vector<std::future<int>> test;
	for (int i = 0; i < 20; ++i) {
		test.push_back(pool.enqueueFunc("testNormalFunc-"+std::to_string(i),0, testF));
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