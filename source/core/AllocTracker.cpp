#include "AllocTracker.hpp"

brk::AllocTracker::AllocTracker(std::pmr::memory_resource& upstream)
	: m_Upstream{ &upstream }
{}

brk::AllocTracker::AllocTracker(AllocTracker&& other)
	: m_Upstream{ other.m_Upstream }
	, m_Info{ other.m_Info }
{
	other.m_Upstream = std::pmr::new_delete_resource();
	other.m_Info = {};
}

brk::AllocTracker& brk::AllocTracker::operator=(AllocTracker&& other)
{
	std::swap(other.m_Upstream, m_Upstream);
	std::swap(m_Info, other.m_Info);
	return *this;
}

void* brk::AllocTracker::do_allocate(size_t n, size_t alignment)
{
	void* res = m_Upstream->allocate(n, alignment);
	if (!res)
		return nullptr;

	++m_Info.m_NumAllocs;
	m_Info.m_TotalSize += n;
	return res;
}

void brk::AllocTracker::do_deallocate(void* ptr, size_t n, size_t alignment)
{
	m_Upstream->deallocate(ptr, n, alignment);
	--m_Info.m_NumAllocs;
	m_Info.m_TotalSize -= n;
}