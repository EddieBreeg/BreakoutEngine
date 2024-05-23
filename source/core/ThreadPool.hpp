#pragma once

#include <PCH.hpp>
#include "Function.hpp"
#include <condition_variable>
#include <queue>
#include <mutex>
#include <thread>
#include <tuple>
#include <vector>

namespace brk {
	/**
	 * Pool of threads capable of running jobs concurrently
	 */
	class ThreadPool
	{
	public:
		explicit ThreadPool(const uint32 threadCount);
		~ThreadPool();

		/**
		 * Starts the threads. This has no effect if the thread pool has been previously
		 * stopped.
		 */
		void Start();
		/**
		 * Waits for all tasks in the queue to be completed.
		 */
		void Wait();
		/**
		 * Stops all running threads, and clears the queue. If a thread is currently
		 * executing a task, it will stop after finishing said task.
		 */
		void Stop();

		/**
		 * Creates a new task that will be picked up by a thread, provided the pool has
		 * been started.
		 */
		template <class Cbk, class... Args>
		void EnqueueTask(Cbk&& callback, Args&&... args);

	private:
		void Loop();

		template <class F, class... Args, size_t... I>
		static void Apply(
			F&& func,
			std::tuple<Args...>&& args,
			std::index_sequence<I...>);

		std::queue<UniqueFunction<void()>> m_Tasks;
		std::mutex m_Mutex;
		std::condition_variable m_Cv;
		uint32 m_CurrentlyRunningTasks = 0;
		std::vector<std::thread> m_Threads;

		enum EState
		{
			Default = 0,
			Started,
			Stopped,
		} m_State = EState::Default;
	};
} // namespace brk

#include "ThreadPool.inl"