#include "ResourceLoader.hpp"
#include "LogManager.hpp"
#include "Resource.hpp"
#include "ResourceFormatter.hpp"

brk::ResourceLoader::~ResourceLoader()
{
	StopDeferred();
	for (auto& t : m_Threads)
		t.join();
}

void brk::ResourceLoader::StopDeferred()
{
	if (!m_Running)
		return;

	m_Running = false;
	std::queue<Job> temp;
	{
		std::unique_lock lock{ m_Mutex };
		m_Jobs.swap(temp);
		m_ActiveRequests = 0;
	}

	m_Cv.notify_all();
}

void brk::ResourceLoader::Wait()
{
	std::unique_lock lock{ m_Mutex };
	while (m_Running && m_ActiveRequests)
	{
		m_Cv.wait(lock);
	}
}

void brk::ResourceLoader::AddJob(Resource* res, bool load)
{
	std::unique_lock lock{ m_Mutex };
	if (!m_Running)
		return;

	m_Jobs.emplace(res, load);
}

void brk::ResourceLoader::ProcessBatch()
{
	{
		std::unique_lock lock{ m_Mutex };
		// if we're already processing a batch, wait until we're done
		if (m_ActiveRequests)
			return;

		m_ActiveRequests = m_Jobs.size();
	}
	m_Cv.notify_all();
}

uint32 brk::ResourceLoader::GetQueueSize()
{
	std::unique_lock lock{ m_Mutex };
	return m_Jobs.size();
}

brk::ResourceLoader::ResourceLoader()
{
	for (auto& th : m_Threads)
	{
		th = std::thread{ &ResourceLoader::Loop, this };
	}
}

void brk::ResourceLoader::Loop()
{
	void (*execFunctions[])(Resource*) = {
		[](Resource* res)
		{
			if (res->m_LoadingState != Resource::Unloading)
				return;
			res->DoUnload();
			res->m_LoadingState = Resource::Unloaded;
		},
		[](Resource* res)
		{
			if (res->m_LoadingState != Resource::Loading)
				return;
			if (res->DoLoad())
			{
				res->m_LoadingState = Resource::Loaded;
				return;
			}
			BRK_LOG_WARNING("Resource::DoLoad returned false for resource {}", *res);
		},
	};

	while (m_Running)
	{
		std::unique_lock lock{ m_Mutex };
		while (m_Running && !m_ActiveRequests)
		{
			m_Cv.wait(lock);
		}
		if (!m_Running)
			return;

		if (!m_ActiveRequests)
			continue;

		while (m_ActiveRequests && m_Running)
		{
			auto& job = m_Jobs.front();
			m_Jobs.pop();
			--m_ActiveRequests;

			lock.unlock();
			(execFunctions[job.second])(job.first);
			m_Cv.notify_all();
			lock.lock();
		}
	}
}
