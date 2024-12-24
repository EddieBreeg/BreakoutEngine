#include "MemoryPool.hpp"
#include "Assert.hpp"
#include "Bitset.hpp"
#include "DebugBreak.hpp"
#include "LogManager.hpp"

#include <memory_resource>

#define PADDING(n, align) (((align) - ((n) % (align))) % (align))

struct brk::MemoryPool::Header
{
	uint32 m_Cap = 0;
	uint32 m_Used = 0;
	Header* m_Next = nullptr;

	static constexpr uint32 GetAllocOffset() noexcept
	{
		return sizeof(Header) + PADDING(sizeof(Header), alignof(std::max_align_t));
	}

	static uint32 CalculateAllocSize(uint32 numBlocks, uint32 blockSize) noexcept
	{
		const uint32 bitsetSize = (numBlocks - 1) / 8 + 1;
		return GetAllocOffset() + (numBlocks * blockSize) + bitsetSize;
	}

	byte* GetChunkStart() noexcept
	{
		return reinterpret_cast<byte*>(this) + GetAllocOffset();
	}

	BitsetView GetBitset(uint32 blockSize)
	{
		return BitsetView{ GetChunkStart() + m_Cap * blockSize, m_Cap };
	}

	void* TryAllocate(uint32 n, uint32 blockSize)
	{
		BitsetView bitset = GetBitset(blockSize);
		const uint32 index = bitset.Find(false, n);
		if (index >= m_Cap)
			return nullptr;

		bitset.Set(index, n);
		m_Used += n;
		return GetChunkStart() + index * blockSize;
	}

	bool TryDeallocate(uint32 index, uint32 n, uint32 blockSize)
	{
		if ((index + n) > m_Cap)
			return false;

		BRK_ASSERT(
			n <= m_Used,
			"Tried to deallocate {} blocks, but chunk is only using {}",
			n,
			m_Used);
		GetBitset(blockSize).Clear(index, n);
		m_Used -= n;
		return true;
	}
};

#undef PADDING

namespace {
	uint32 GrowSize(uint32 size)
	{
		return (3 * size - 1) / 2 + 1;
	}
} // namespace

brk::MemoryPool::MemoryPool(uint32 blockSize) noexcept
	: MemoryPool(blockSize, 0, *std::pmr::new_delete_resource())
{}

brk::MemoryPool::MemoryPool(uint32 blockSize, uint32 initBlockCount)
	: MemoryPool(blockSize, initBlockCount, *std::pmr::new_delete_resource())
{}

brk::MemoryPool::MemoryPool(
	uint32 blockSize,
	uint32 initBlockCount,
	std::pmr::memory_resource& upstreamResource)
	: m_Upstream{ &upstreamResource }
	, m_BlockSize{ blockSize }
{
	BRK_ASSERT(blockSize, "Block size must be non 0");
	if (initBlockCount)
		m_Head = AllocateNewChunk(initBlockCount);
}

brk::MemoryPool::MemoryPool(MemoryPool&& other) noexcept
	: m_Upstream{ other.m_Upstream }
	, m_Head{ other.m_Head }
	, m_BlockSize{ other.m_BlockSize }
{
	other.m_Head = nullptr;
}

brk::MemoryPool::~MemoryPool()
{
	Reset();
}

brk::MemoryPool& brk::MemoryPool::operator=(MemoryPool&& other) noexcept
{
	std::swap(m_Upstream, other.m_Upstream);
	std::swap(m_Head, other.m_Head);
	std::swap(m_BlockSize, other.m_BlockSize);
	return *this;
}

void brk::MemoryPool::Reset()
{
	while (m_Head)
	{
		m_Head = DeallocateChunk(m_Head);
	}
}

void brk::MemoryPool::Clear()
{
	for (Header* chunk = m_Head; chunk; chunk = chunk->m_Next)
	{
		chunk->m_Used = 0;
		chunk->GetBitset(m_BlockSize).ClearAll();
	}
}

void* brk::MemoryPool::Allocate(uint32 n)
{
	if (!m_Head)
	{
		m_Head = AllocateNewChunk(n, true);
		return m_Head->GetChunkStart();
	}

	for (Header* it = m_Head; it; it = it->m_Next)
	{
		if (void* ptr = it->TryAllocate(n, m_BlockSize))
			return ptr;
	}

	uint32 allocSize = GrowSize(m_Head->m_Cap);
	if (allocSize < n)
		allocSize = n;

	Header* newHeader = AllocateNewChunk(allocSize, true);
	newHeader->m_Next = m_Head;
	m_Head = newHeader;
	return newHeader->GetChunkStart();
}

void brk::MemoryPool::Deallocate(void* ptr, uint32 n)
{
	for (Header* it = m_Head; it; it = it->m_Next)
	{
		const uint32 index =
			uint32(static_cast<byte*>(ptr) - it->GetChunkStart()) / m_BlockSize;

		if (it->TryDeallocate(index, n, m_BlockSize))
			return;
	}
	BRK_LOG_CRITICAL("Failed to deallocate: {} doesn't belong to this pool", ptr);
	dbg::Break();
}

brk::MemoryPool::Header* brk::MemoryPool::AllocateNewChunk(
	uint32 capacity,
	bool allocateBlocks)
{
	void* ptr = m_Upstream->allocate(
		Header::CalculateAllocSize(capacity, m_BlockSize),
		alignof(Header));
	Header* chunk = new (ptr) Header{ capacity };
	BitsetView bitset = chunk->GetBitset(m_BlockSize);

	if (allocateBlocks)
	{
		bitset.SetAll();
		chunk->m_Used = capacity;
	}
	else
	{
		bitset.ClearAll();
	}

	return chunk;
}

brk::MemoryPool::Header* brk::MemoryPool::DeallocateChunk(Header* chunk)
{
	Header* next = chunk->m_Next;
	m_Upstream->deallocate(
		chunk,
		Header::CalculateAllocSize(chunk->m_Cap, m_BlockSize),
		alignof(Header));
	return next;
}
