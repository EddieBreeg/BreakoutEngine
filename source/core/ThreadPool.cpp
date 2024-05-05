#include "ThreadPool.hpp"

brk::ThreadPool::ThreadPool(const uint32 threadCount)
	: m_Threads{ threadCount }
{
	for (std::thread& t : m_Threads)
	{
		t = std::thread(&ThreadPool::Loop, this);
	}
}

brk::ThreadPool::~ThreadPool()
{
	Stop();
	for (std::thread& t : m_Threads)
	{
		t.join();
	}
}

void brk::ThreadPool::Wait()
{
	std::unique_lock lock{ m_Mutex };
	while (m_State == EState::Started && (m_Tasks.size() || m_CurrentlyRunningTasks))
		m_Cv.wait(lock);
}

void brk::ThreadPool::Start()
{
	{
		std::unique_lock lock{ m_Mutex };
		m_State = EState::Started;
	}
	m_Cv.notify_all();
}

void brk::ThreadPool::Stop()
{
	std::queue<UniqueFunction<void()>> tmp;
	{
		std::unique_lock lock{ m_Mutex };
		if (m_State == EState::Stopped)
			return;
		m_Tasks.swap(tmp);
		m_State = EState::Stopped;
	}
	m_Cv.notify_all();
}

void brk::ThreadPool::Loop()
{
	while (true)
	{
		UniqueFunction<void()> task;
		{
			std::unique_lock lock{ m_Mutex };
			while (m_State == EState::Default || (m_State == EState::Started && m_Tasks.empty()))
				m_Cv.wait(lock);

			if (m_State == EState::Stopped)
				return;

			task = std::move(m_Tasks.front());
			m_Tasks.pop();
			++m_CurrentlyRunningTasks;
		}
		task();
		{
			std::unique_lock lock{ m_Mutex };
			--m_CurrentlyRunningTasks;
		}
		m_Cv.notify_all();
	}
}
