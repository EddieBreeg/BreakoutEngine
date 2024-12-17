#include <core/MemoryPool.hpp>

namespace brk::memory::ut {
	struct MemoryProvider : public std::pmr::memory_resource
	{
		MemoryProvider() = default;
		size_t m_TotalSize = 0;

	protected:
		bool do_is_equal(const std::pmr::memory_resource&) const noexcept override
		{
			return false;
		}

		void* do_allocate(size_t n, size_t) override
		{
			m_TotalSize += n;
			return new byte[n];
		}

		void do_deallocate(void* ptr, size_t n, size_t) override
		{
			m_TotalSize -= n;
			delete[] ptr;
		}
	};

	void PoolTests()
	{
		using TPool = MemoryPool<8, 4>;
		using TBlock = TPool::Block;
		MemoryProvider upstream;

		{
			TPool pool{ 10, &upstream };
			assert(upstream.m_TotalSize > 0);
		}
		assert(upstream.m_TotalSize == 0);
		{
			TPool pool{ 10, &upstream };
			auto total = upstream.m_TotalSize;
			void* start1 = pool.Allocate(2);
			assert(start1);
			assert(total == upstream.m_TotalSize);

			pool.Deallocate(start1, 1);
			void* ptr = pool.Allocate(1);
			assert(ptr == start1);
		}

		TPool temp;
		TBlock *start1 = nullptr, *start2 = nullptr;
		{
			TPool pool{ 1, &upstream };
			auto total = upstream.m_TotalSize;

			start1 = static_cast<TBlock*>(pool.Allocate(1));
			assert(start1);

			start2 = static_cast<TBlock*>(pool.Allocate(10));
			assert(start2);

			assert(upstream.m_TotalSize > total);
			total = upstream.m_TotalSize;

			pool.Clear();
			assert(pool.Allocate(1) == start2);
			assert(upstream.m_TotalSize == total);

			assert(pool.Allocate(9) == (start2 + 1));
			assert(pool.Allocate(1) == start1);
			assert(upstream.m_TotalSize == total);

			temp = std::move(pool);
		}

		assert(upstream.m_TotalSize);
		temp.Clear();
		assert(temp.Allocate(10) == start2);
		assert(temp.Allocate(1) == start1);
		assert(temp.GetUpstream() == upstream);
	}
} // namespace brk::memory::ut