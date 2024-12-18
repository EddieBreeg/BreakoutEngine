#pragma once

#include <PCH.hpp>
#include <memory_resource>

namespace brk {
	/**
	 * Generic pool allocation primitive. A pool maintains a stack of chunks,
	 * which are divided into blocks of even size. The state of these blocks
	 * (available/used) is stored in a bitset, which is allocated along with the chunk
	 * itself. When an allocation is requested and the pool doesn't have enough free
	 * space, a new chunk is allocated using an upstream memory resource. Chunk size grows
	 * geometrically.
	 */
	template <uint32 BlockSize, uint32 Alignment = alignof(std::max_align_t)>
	class MemoryPool
	{
	public:
		static_assert(
			(Alignment & (~Alignment + 1)) == Alignment,
			"Alignment must be a power of two");
		static_assert(BlockSize);

		struct Block
		{
			alignas(Alignment) byte m_Block[BlockSize];
		};

		/**
		 * Constructs a new empty pool with the default upstream resource (which uses
		 * new/delete)
		 */
		MemoryPool() noexcept = default;
		/**
		 * Constructs a pool with a pre-allocated chunk and the provided memory upstream
		 * resource.
		 * \param initialBlockCount: The number of blocks in the pre-allocated chunk
		 * \param upstream: The upstream resource to use for allocations, when needed
		 */
		MemoryPool(uint32 initialBlockCount, std::pmr::memory_resource* upstream);
		/**
		 * Constructs a pool with a pre-allocated chunk using the default upstream
		 * resource (which uses new/delete).
		 * \param initialBlockCount: The number of blocks in the pre-allocated chunk
		 */
		MemoryPool(uint32 initialBlockCount);
		MemoryPool(const MemoryPool&) = delete;

		/**
		 * \note After this constructor, other is empty but still retains its
		 * upstream resource.
		 */
		MemoryPool(MemoryPool&& other);

		MemoryPool& operator=(const MemoryPool&) = delete;
		/**
		 * \note Swaps this with other.
		 */
		MemoryPool& operator=(MemoryPool&& other);

		/**
		 * Calls Reset
		 */
		~MemoryPool();

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

		[[nodiscard]] std::pmr::memory_resource& GetUpstream() const noexcept;

		// Marks all allocated blocks as free, without releasing the memory
		void Clear();
		// Releases all allocated memory
		void Reset();

	private:
		struct Header;

		Header* AllocateNewChunk(uint32 count);
		Header* DeallocateChunk(Header* chunk);

	private:
		std::pmr::memory_resource* m_Upstream = std::pmr::new_delete_resource();
		Header* m_Head = nullptr;
	};

	/**
	 * Defines a memory pool suitable for a given type T, assuming T is complete.
	 */
	template <class T>
	using TypedMemoryPool = MemoryPool<sizeof(T), alignof(T)>;

	/**
	 * Opaque memory pool, using type erasure.
	 * This class implements the standard std::pmr::memory_resource interface,
	 * and thus inherits the allocate/deallocate functions.
	 * \warning Don't use allocate/deallocate in non-polymorphic contexts!
	 * Allocate/Deallocate should be preferred.
	 */
	class BRK_CORE_API PolymorphicMemoryPool : public std::pmr::memory_resource
	{
	public:
		PolymorphicMemoryPool() noexcept = default;

		PolymorphicMemoryPool(const PolymorphicMemoryPool&) = delete;
		/**
		 * After this call, other.IsValid() returns false
		 */
		PolymorphicMemoryPool(PolymorphicMemoryPool&& other);

		PolymorphicMemoryPool& operator=(const PolymorphicMemoryPool&) = delete;
		/**
		 * Effectively swaps *this this other
		 */
		PolymorphicMemoryPool& operator=(PolymorphicMemoryPool&&);

		/**
		 * Calls Reset
		 */
		~PolymorphicMemoryPool();

		/**
		 * Constructs a pool of the provided type
		 * \tparam B: The block size to use
		 * \tparam A: The block alignment
		 * \param initBlockCount: The initial block count to allocate. If 0, no chunk
		 * will be allocated.
		 * \param upstream: The upstream resource to use for additionnal allocations
		 */
		template <uint32 B, uint32 A>
		PolymorphicMemoryPool(
			InPlaceType<MemoryPool<B, A>>,
			uint32 initBlockCount,
			std::pmr::memory_resource* upstream = std::pmr::new_delete_resource());

		/**
		 * Constructs from a pool object. After this call,
		 * pool is still valid and still holds a pointer the upstream resource
		 * it was using before, but it will be empty.
		 */
		template <uint32 B, uint32 A>
		PolymorphicMemoryPool(MemoryPool<B, A>&& pool);

		/**
		 * Allocates from the internal pool.
		 * \warning Asserts if no pool is stored
		 */
		void* Allocate(uint32 n);
		/**
		 * Deallocates from the internal pool.
		 * \warning Asserts if no pool is stored
		 */
		void Deallocate(void* ptr, uint32 n);

		[[nodiscard]] bool IsValid() const noexcept { return m_BlockSize; }

		/**
		 * Clears the contents of the pool, without releasing the memory.
		 * If no pool is actually stored, does nothing.
		 */
		void Clear();
		/**
		 * Releases all allocates blocks.
		 * If no pool is actually stored, does nothing.
		 */
		void Reset();

	protected:
		void* do_allocate(size_t size, size_t alignment) override;
		void do_deallocate(void* ptr, size_t size, size_t alignment) override;
		bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

	private:
		struct
		{
			void* m_Buf[2] = { nullptr };
		} m_Storage;
		uint32 m_BlockSize = 0, m_Alignment = 0;
		void* (*m_Allocate)(void* pool, uint32 n) = nullptr;
		void (*m_Deallocate)(void* pool, void* ptr, uint32 n) = nullptr;
		void (*m_Clear)(void*, bool) = nullptr;
	};
} // namespace brk