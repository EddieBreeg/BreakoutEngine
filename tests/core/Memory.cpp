#include <core/AllocTracker.hpp>
#include <core/MemoryPool.hpp>

namespace brk::memory::ut {
	void PoolTests()
	{
		struct Block
		{
			byte m_Buf[8];
		};
		AllocTracker upstream;

		{
			MemoryPool pool{ sizeof(Block), 10, upstream };
			assert(upstream.GetInfo().m_NumAllocs == 1);
		}
		assert(upstream.GetInfo().m_NumAllocs == 0);
		assert(upstream.GetInfo().m_TotalSize == 0);

		{
			MemoryPool pool{ sizeof(Block), 10, upstream };
			void* start1 = pool.Allocate(2);
			assert(start1);
			assert(upstream.GetInfo().m_NumAllocs == 1);

			pool.Deallocate(start1, 1);
			void* ptr = pool.Allocate(1);
			assert(ptr == start1);
		}

		MemoryPool temp{ sizeof(Block) };
		Block *start1 = nullptr, *start2 = nullptr;
		{
			MemoryPool pool{ sizeof(Block), 1, upstream };

			start1 = static_cast<Block*>(pool.Allocate(1));
			assert(start1);

			start2 = static_cast<Block*>(pool.Allocate(10));
			assert(start2);

			assert(upstream.GetInfo().m_NumAllocs == 2);

			pool.Clear();
			assert(pool.Allocate(1) == start2);
			assert(upstream.GetInfo().m_NumAllocs == 2);

			assert(pool.Allocate(9) == (start2 + 1));
			assert(pool.Allocate(1) == start1);
			assert(upstream.GetInfo().m_NumAllocs == 2);

			temp = std::move(pool);
		}

		assert(upstream.GetInfo().m_NumAllocs == 2);
		temp.Clear();
		assert(temp.Allocate(10) == start2);
		assert(temp.Allocate(1) == start1);
		assert(temp.GetUpstream() == &upstream);
		temp.Reset();
		assert(upstream.GetInfo().m_NumAllocs == 0);

		temp.Allocate(1);
		assert(upstream.GetInfo().m_NumAllocs == 1);
		temp.Allocate(1);
		assert(upstream.GetInfo().m_NumAllocs == 2);
	}
} // namespace brk::memory::ut