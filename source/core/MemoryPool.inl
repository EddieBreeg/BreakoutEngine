namespace brk {
	template <uint32 B, uint32 A>
	struct MemoryPool<B, A>::Header
	{
		Header* m_Next = nullptr;
		uint32 m_Cap, m_Used = 0;
		Block m_Buf[1];

		void* TryAllocate(uint32 n);
		BitsetView GetBitset() noexcept;
		void Deallocate(uint32 index, uint32 n);
	};

	template <uint32 BlockSize, uint32 Alignment>
	inline MemoryPool<BlockSize, Alignment>::MemoryPool(
		uint32 initialBlockCount,
		std::pmr::memory_resource* upstream)
		: m_Upstream{ upstream }
	{
		if (initialBlockCount)
			AllocateNewChunk(initialBlockCount);
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline MemoryPool<BlockSize, Alignment>::MemoryPool(uint32 initialBlockCount)
		: MemoryPool{ initialBlockCount, std::pmr::new_delete_resource() }
	{}

	template <uint32 BlockSize, uint32 Alignment>
	inline MemoryPool<BlockSize, Alignment>::MemoryPool(MemoryPool&& other)
		: m_Upstream{ other.m_Upstream }
		, m_Head{ other.m_Head }
	{
		other.m_Head = nullptr;
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline MemoryPool<BlockSize, Alignment>& MemoryPool<BlockSize, Alignment>::operator=(
		MemoryPool&& other)
	{
		std::swap(m_Head, other.m_Head);
		std::swap(m_Upstream, other.m_Upstream);
		return *this;
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline MemoryPool<BlockSize, Alignment>::~MemoryPool()
	{
		Reset();
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline void* MemoryPool<BlockSize, Alignment>::Allocate(uint32 n)
	{
		for (Header* chunk = m_Head; chunk; chunk = chunk->m_Next)
		{
			if (void* ptr = chunk->TryAllocate(n))
				return ptr;
		}
		uint32 nextSize = m_Head->m_Cap * 3 / 2;
		if (nextSize < n)
			nextSize = n;

		return AllocateNewChunk(nextSize)->TryAllocate(n);
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline void MemoryPool<BlockSize, Alignment>::Deallocate(void* ptr, uint32 n)
	{
		for (Header* chunk = m_Head; chunk; chunk = chunk->m_Next)
		{
			const uint32 index = static_cast<Block*>(ptr) - chunk->m_Buf;
			if (index < chunk->m_Cap)
			{
				chunk->Deallocate(index, n);
				return;
			}
		}
		BRK_LOG_CRITICAL("Deallocation failed for {}", ptr);
		dbg::Break();
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline std::pmr::memory_resource& MemoryPool<BlockSize, Alignment>::GetUpstream()
		const noexcept
	{
		return *m_Upstream;
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline void MemoryPool<BlockSize, Alignment>::Clear()
	{
		for (Header* chunk = m_Head; chunk; chunk = chunk->m_Next)
		{
			chunk->m_Used = 0;
			chunk->GetBitset().ClearAll();
		}
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline void MemoryPool<BlockSize, Alignment>::Reset()
	{
		while (m_Head)
		{
			m_Head = DeallocateChunk(m_Head);
		}
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline typename MemoryPool<BlockSize, Alignment>::Header* MemoryPool<
		BlockSize,
		Alignment>::AllocateNewChunk(uint32 count)
	{
		const uint32 bitsetSize = (count - 1) / 8 + 1;
		const uint32 allocSize = sizeof(Header) + BlockSize * (count - 1) + bitsetSize;
		void* const hdr = m_Upstream->allocate(allocSize, alignof(Header));
		new (hdr) Header{ m_Head, count };
		m_Head = static_cast<Header*>(hdr);
		m_Head->GetBitset().ClearAll();

		return m_Head;
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline typename MemoryPool<BlockSize, Alignment>::Header* MemoryPool<
		BlockSize,
		Alignment>::DeallocateChunk(Header* chunk)
	{
		Header* const next = chunk->m_Next;
		const uint32 bitsetSize = (chunk->m_Cap - 1) / 8 + 1;
		const uint32 size = sizeof(Header) + BlockSize * (chunk->m_Cap - 1) + bitsetSize;
		m_Upstream->deallocate(chunk, size, alignof(Header));
		return next;
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline void* MemoryPool<BlockSize, Alignment>::do_allocate(size_t size, size_t)
	{
		return Allocate(uint32((size - 1) / BlockSize) + 1);
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline void MemoryPool<BlockSize, Alignment>::do_deallocate(
		void* ptr,
		size_t size,
		size_t alignment)
	{
		return Deallocate(ptr, uint32((size - 1) / BlockSize) + 1);
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline bool MemoryPool<BlockSize, Alignment>::do_is_equal(
		const std::pmr::memory_resource& other) const noexcept
	{
		return &other == this;
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline void* MemoryPool<BlockSize, Alignment>::Header::TryAllocate(uint32 n)
	{
		if ((m_Used + n) > m_Cap)
			return nullptr;

		BitsetView blockStates = GetBitset();
		const uint32 index = blockStates.Find(false, n);
		if (index >= m_Cap)
			return nullptr;

		blockStates.Set(index, n);
		m_Used += n;
		return m_Buf + index;
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline BitsetView MemoryPool<BlockSize, Alignment>::Header::GetBitset() noexcept
	{
		return BitsetView{ m_Buf + m_Cap, m_Cap };
	}

	template <uint32 BlockSize, uint32 Alignment>
	inline void MemoryPool<BlockSize, Alignment>::Header::Deallocate(
		uint32 index,
		uint32 n)
	{
		BRK_ASSERT(
			n <= m_Used,
			"Tried to deallocate {} blocks from a chunk which is only using {}",
			n,
			m_Used);
		m_Used -= n;
		GetBitset().Clear(index, n);
	}
} // namespace brk
