#include <core/ThreadPool.hpp>
#include <iostream>
#include <cassert>

namespace brk::threadpool::ut {
	void Tests()
	{
		{
			int result = 0;
			ThreadPool tp{ 1 };
			tp.EnqueueTask(
				[&](const int x)
				{
					result = x;
				},
				1);
			tp.Start();
			tp.Wait();
			assert(result == 1);
		}
		{
			ThreadPool tp{ 1 };
			int result = 0;
			tp.Start();
			tp.EnqueueTask(
				[&](const int& x)
				{
					result = x + 1;
				},
				std::ref(result));
			tp.Wait();
			assert(result == 1);
		}
		{
			const int threadCount = 3;
			ThreadPool tp{ threadCount };
			std::mutex mutex;
			std::condition_variable cv;
			int n = 0;
			auto f = [&]()
			{
				{
					std::lock_guard lock{ mutex };
					++n;
				}
				cv.notify_all();
				std::unique_lock lock{ mutex };
				while (n < threadCount)
					cv.wait(lock);
			};
			for (int i = 0; i < threadCount; ++i)
				tp.EnqueueTask(f);
			tp.Start();
			tp.Wait();
			assert(n == threadCount);
		}
	}
} // namespace brk::threadpool::ut
