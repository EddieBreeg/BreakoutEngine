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
		m_PendingRequests = 0;
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

void brk::ResourceLoader::AddJob(Resource* res, EJobType action)
{
	BRK_ASSERT(
		action > EJobType::Invalid && action < EJobType::NTypes,
		"Invalid job type");
	if (action == Reload)
		res->SetLoadingState(Resource::Reloading);
	std::unique_lock lock{ m_Mutex };
	if (!m_Running)
		return;

	m_Jobs.emplace(res, action);
}

void brk::ResourceLoader::ProcessBatch()
{
	{
		std::unique_lock lock{ m_Mutex };
		// if we're already processing a batch, wait until we're done
		if (m_ActiveRequests)
			return;

		m_ActiveRequests = m_PendingRequests = static_cast<uint32>(m_Jobs.size());
	}
	m_Cv.notify_all();
}

uint32 brk::ResourceLoader::GetQueueSize()
{
	std::unique_lock lock{ m_Mutex };
	return static_cast<uint32>(m_Jobs.size());
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
	void (*execFunctions[EJobType::NTypes])(Resource*) = {
		[](Resource* res)
		{
			if (res->GetLoadingState() != Resource::Unloading)
				return;
			res->DoUnload();
			res->SetLoadingState(Resource::Unloaded);
			BRK_LOG_TRACE("Unloaded {}", *res);
		},
		[](Resource* res)
		{
			if (res->GetLoadingState() != Resource::Loading)
				return;
			if (res->DoLoad())
			{
				res->SetLoadingState(Resource::Loaded);
				BRK_LOG_TRACE("Successfully loaded {}", *res);
				return;
			}
			BRK_LOG_WARNING("Resource::DoLoad returned false for resource {}", *res);
		},
		[](Resource* res)
		{
			if (res->GetLoadingState() != Resource::Reloading)
				return;
			res->DoUnload();
			if (res->DoLoad())
			{
				res->SetLoadingState(Resource::Loaded);
				BRK_LOG_TRACE("Successfully reloaded {}", *res);
				return;
			}
			BRK_LOG_WARNING("Reload failed for resource {}", *res);
		},
	};

	while (m_Running)
	{
		{
			std::unique_lock lock{ m_Mutex };
			while (m_Running && !m_PendingRequests)
			{
				m_Cv.wait(lock);
			}
			if (!m_Running)
				return;

			if (!m_PendingRequests)
				continue;
		}

		while (m_PendingRequests && m_Running)
		{
			--m_PendingRequests;
			Job job;
			{
				std::unique_lock lock{ m_Mutex };

				job = m_Jobs.front();
				m_Jobs.pop();
			}

			(execFunctions[job.second])(job.first);
			{
				std::unique_lock lock{ m_Mutex };
				--m_ActiveRequests;
			}
			m_Cv.notify_all();
		}
	}
}
