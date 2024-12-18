#include "MemoryPoolFwd.hpp"
#include "Assert.hpp"

brk::PolymorphicMemoryPool::PolymorphicMemoryPool(PolymorphicMemoryPool&& other)
	: m_Storage{ other.m_Storage }
	, m_BlockSize{ other.m_BlockSize }
	, m_Alignment{ other.m_Alignment }
	, m_Allocate{ other.m_Allocate }
	, m_Deallocate{ other.m_Deallocate }
	, m_Clear{ other.m_Clear }
{
	other.m_BlockSize = 0;
	other.m_Alignment = 0;
	other.m_Allocate = nullptr;
	other.m_Deallocate = nullptr;
	other.m_Clear = nullptr;
}

brk::PolymorphicMemoryPool& brk::PolymorphicMemoryPool::operator=(
	PolymorphicMemoryPool&& other)
{
	std::swap(m_Storage, other.m_Storage);
	std::swap(m_BlockSize, other.m_BlockSize);
	std::swap(m_Alignment, other.m_Alignment);
	std::swap(m_Allocate, other.m_Allocate);
	std::swap(m_Deallocate, other.m_Deallocate);
	std::swap(m_Clear, other.m_Clear);
	return *this;
}

brk::PolymorphicMemoryPool::~PolymorphicMemoryPool()
{
	Reset();
}

void* brk::PolymorphicMemoryPool::Allocate(uint32 n)
{
	BRK_ASSERT(m_Allocate, "Called Allocate on an invalid pool");
	return m_Allocate(m_Storage.m_Buf, n);
}

void brk::PolymorphicMemoryPool::Deallocate(void* ptr, uint32 n)
{
	BRK_ASSERT(m_Deallocate, "Called Deallocate on an invalid pool");
	return m_Deallocate(m_Storage.m_Buf, ptr, n);
}

void brk::PolymorphicMemoryPool::Clear()
{
	if (!m_Clear)
		return;

	m_Clear(m_Storage.m_Buf, false);
}

void brk::PolymorphicMemoryPool::Reset()
{
	if (!m_Clear)
		return;

	m_Clear(m_Storage.m_Buf, true);
}
