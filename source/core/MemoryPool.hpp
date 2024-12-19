#pragma once

#include <PCH.hpp>
#include <memory_resource>

namespace brk {
	class BRK_CORE_API MemoryPool
	{
	public:
		explicit MemoryPool(uint32 blockSize) noexcept;
		MemoryPool(uint32 blockSize, uint32 initBlockCount);
		MemoryPool(
			uint32 blockSize,
			uint32 initBlockCount,
			std::pmr::memory_resource& upstreamResource);

		MemoryPool(const MemoryPool&) = delete;
		MemoryPool(MemoryPool&& other) noexcept;

		~MemoryPool();

		MemoryPool& operator=(const MemoryPool&) = delete;
		MemoryPool& operator=(MemoryPool&& other) noexcept;

		void Reset();
		void Clear();

		/**
		 * Allocates n blocks. If a pre-allocated chunk has enough contiguous space to
		 * accomodate the requested number of blocks, these blocks are marked as used and
		 * their address is returned. If no chunk could fit the requested size, a new
		 * chunk is allocated from the upstream resource.
		 * \note Chunk size grows geometrically, and is guaranteed to be big enough for
		 * the requested allocation. When looking for a suitable chunk, chunks are
		 * iterated through in reverse creation order (meaning the biggest chunk first, as
		 * it is likely to be the one with the most space available).
		 */
		void* Allocate(uint32 n);

		/**
		 * Deallocates a pointer returned by Allocate.
		 * \param ptr: The pointer to the memory area to deallocate. Must have been
		 * allocated by this pool.
		 * \param n: The number of blocks to deallocate. This is allowed to be smaller
		 * than the original allocated size, but behaviour is undefined if it exceeds it.
		 * \warning This function will assert if ptr doesn't belong to this pool.
		 */
		void Deallocate(void* ptr, uint32 n);

		[[nodiscard]] uint32 GetBlockSize() const noexcept { return m_BlockSize; }
		[[nodiscard]] std::pmr::memory_resource* GetUpstream() const noexcept
		{
			return m_Upstream;
		}

	protected:
	private:
		struct Header;
		Header* AllocateNewChunk(uint32 capacity, bool allocateBlocks = false);
		Header* DeallocateChunk(Header* chunk);

		std::pmr::memory_resource* m_Upstream = std::pmr::new_delete_resource();
		Header* m_Head = nullptr;
		uint32 m_BlockSize;
	};
} // namespace brk