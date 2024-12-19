#pragma once

#include <PCH.hpp>

namespace std::pmr {
	class memory_resource;
}

namespace brk {
	/**
	 * Simple memory pool implementation.
	 * \details A pool consists of a stack of chunks, divided
	 * into evenly sized blocks. Each chunk maintains a bitset, which records the state of
	 * every block (available or used). When no chunk can hold the number of requested
	 * blocks for an allocation, a new chunk is allocated using an upstream resource.
	 * \warning This implementation is not thread safe.
	 */
	class BRK_CORE_API MemoryPool
	{
	public:
		/** Constructs an empty pool with the provided block size, which uses
		 * new/delete as its upstream resource.
		 * \warning Asserts if blockSize is 0
		 */
		explicit MemoryPool(uint32 blockSize) noexcept;
		/** Constructs an pool with the given blocksize, and preallocates a chunk.
		 * This overload sets the upstream resource to new/delete.
		 * \param blockSize: The size of blocks which make up the chunks.
		 * \param initBlockCount: The capacity of the preallocated chunk.
		 * \warning Asserts if blockSize is 0
		 */
		MemoryPool(uint32 blockSize, uint32 initBlockCount);
		/** Constructs a pool with the given block size and upstream resource,
		 * and preallocates a chunk.
		 * \param blockSize: The size of blocks which make up the chunks.
		 * \param initBlockCount: The capacity of the preallocated chunk.
		 * \param upstreamResource: The memory resource to be used when *additionnal
		 * memory is required for a new chunk.
		 * \warning Asserts if blockSize is 0
		 */
		MemoryPool(
			uint32 blockSize,
			uint32 initBlockCount,
			std::pmr::memory_resource& upstreamResource);

		MemoryPool(const MemoryPool&) = delete;
		/**
		 * After this call, other is empty but still retains its upstream resource and
		 * block size. This is so that a pool can still be used after a move.
		 */
		MemoryPool(MemoryPool&& other) noexcept;

		/**
		 * Calls Reset
		 */
		~MemoryPool();

		MemoryPool& operator=(const MemoryPool&) = delete;
		/**
		 * Swaps *this with other.
		 */
		MemoryPool& operator=(MemoryPool&& other) noexcept;

		/**
		 * Releases all allocated memory from the upstream resource.
		 * After this call, the pool still retains the pointer to its upstream resource
		 * and blocksize, and can therefore be used again at any time.
		 */
		void Reset();
		/**
		 * Makes all previously allocated blocks available, without releasing the memory.
		 * Of course, destroying the objects stored inside the blocks is still the
		 * responsibility of the caller.
		 */
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
		 * \warning Alignment is determined by the block size. You should only allocate objects
		 * with a pool where the block size is a multiple of your object size (or vice versa),
		 * lest your objects be incorrectly aligned in some cases.
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

	private:
		struct Header;
		Header* AllocateNewChunk(uint32 capacity, bool allocateBlocks = false);
		Header* DeallocateChunk(Header* chunk);

		std::pmr::memory_resource* m_Upstream;
		Header* m_Head = nullptr;
		uint32 m_BlockSize;
	};
} // namespace brk