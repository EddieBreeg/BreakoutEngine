#pragma once

#include <PCH.hpp>

#include "Singleton.hpp"

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace brk {
	class Resource;

	class BRK_CORE_API ResourceLoader : public Singleton<ResourceLoader>
	{
	public:
		/** Stops and joins the threads */
		~ResourceLoader();
		/**
		 * Clears the internal queue, and notifies the threads. All available threads exit
		 * as soon they receive the notification. Currently working threads will exit when
		 * they're done with whatever job they were processing.
		 * Unblocks on current calls to Wait.
		 * \warning After calling this function, AddJob and ProcessBatch are no-ops.
		 */
		void StopDeferred();
		/**
		 * Blocks until either all active jobs have been completed, or Stop is called.
		 */
		void Wait();

		/**
		 * Adds a resource load/unload request to the internal queue. Such requests are
		 * only processed after ProcessBatch has been called.
		 * \param res: The resource to load/unload
		 * \param load: Whether the resource should be loaded (true) or unloaded (false)
		 * \warning This function has no effect if the loader is stopped
		 */
		void AddJob(Resource* res, bool load = true);

		/**
		 * Starts processing all jobs currently in the queue, and returns without
		 * blocking. If AddJob is called while processing active jobs, the newly created
		 * jobs will not be processed until the current batch is over and ProcessBatch is
		 * called anew.
		 * \warning This has no effect if the loader is stopped
		 */
		void ProcessBatch();

		/**
		 * Returns the total number of jobs currently in the queue
		 */
		[[nodiscard]] uint32 GetQueueSize();

	private:
		ResourceLoader();
		friend class Singleton<ResourceLoader>;
		static inline std::unique_ptr<ResourceLoader> s_Instance;

		void Loop();

		static constexpr uint32 s_NumThreads = 4;
		using Job = std::pair<Resource*, bool>;
		std::queue<Job> m_Jobs;

		std::thread m_Threads[s_NumThreads];
		std::mutex m_Mutex;
		std::condition_variable m_Cv;

		uint32 m_ActiveRequests = 0;
		std::atomic_uint32_t m_PendingRequests = 0;
		std::atomic_bool m_Running = true;
	};
} // namespace brk