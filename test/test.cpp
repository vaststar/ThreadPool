#include "ThreadPool.h"

#include <iostream>

int exexp(int i)
{
	std::cout << "hello world " << i << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));

	return i * i;
}
int main()
{
	{
		ThreadPool pool(4);
		for (int i = 0; i < 20; ++i) {
		pool.PushFunc(exexp,i);
		}
	}
	return 0;
}