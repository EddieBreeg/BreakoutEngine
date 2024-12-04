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
		~ResourceLoader();
		void StopDeferred();
		void Wait();

		void AddJob(Resource* res, bool load = true);

		void ProcessBatch();

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

		uint32 m_ActiveRequests = 0; // number of requests in the current batch
		std::atomic_bool m_Running = true;
	};
} // namespace brk