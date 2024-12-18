#include <core/AllocTracker.hpp>
#include <core/MemoryPool.hpp>

namespace brk::memory::ut {
	void PolyPoolTests();

	void PoolTests()
	{
		using TPool = MemoryPool<8, 4>;
		using TBlock = TPool::Block;
		AllocTracker upstream;

		{
			TPool pool{ 10, &upstream };
			assert(upstream.GetInfo().m_NumAllocs == 1);
		}
		assert(upstream.GetInfo().m_NumAllocs == 0);
		assert(upstream.GetInfo().m_TotalSize == 0);

		{
			TPool pool{ 10, &upstream };
			void* start1 = pool.Allocate(2);
			assert(start1);
			assert(upstream.GetInfo().m_NumAllocs == 1);

			pool.Deallocate(start1, 1);
			void* ptr = pool.Allocate(1);
			assert(ptr == start1);
		}

		TPool temp;
		TBlock *start1 = nullptr, *start2 = nullptr;
		{
			TPool pool{ 1, &upstream };

			start1 = static_cast<TBlock*>(pool.Allocate(1));
			assert(start1);

			start2 = static_cast<TBlock*>(pool.Allocate(10));
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
		assert(temp.GetUpstream() == upstream);

		PolyPoolTests();
	}

	void PolyPoolTests()
	{
		AllocTracker upstream;
		using TPool = MemoryPool<8, 8>;
		{
			const PolymorphicMemoryPool pool;
			assert(!pool.IsValid());
		}
		{
			PolymorphicMemoryPool pool{ InPlace<TPool>, 0, &upstream };
			assert(pool.IsValid());
			assert(upstream.GetInfo().m_NumAllocs == 0);
		}
		{
			PolymorphicMemoryPool pool{ InPlace<TPool>, 10, &upstream };
			assert(pool.IsValid());
			assert(upstream.GetInfo().m_NumAllocs == 1);

			pool.Clear();
			assert(upstream.GetInfo().m_NumAllocs == 1);

			pool.Reset();
			assert(upstream.GetInfo().m_TotalSize == 0);
		}
		{
			PolymorphicMemoryPool p1{ InPlace<TPool>, 0, &upstream };
			PolymorphicMemoryPool p2{ std::move(p1) };
			assert(!p1.IsValid());
			assert(p2.IsValid());
		}
		{
			PolymorphicMemoryPool p1{ InPlace<TPool>, 1, &upstream };
			assert(upstream.GetInfo().m_NumAllocs == 1);

			PolymorphicMemoryPool p2;
			p2 = std::move(p1);
			assert(!p1.IsValid());
			assert(p2.IsValid());
			assert(upstream.GetInfo().m_NumAllocs == 1);
		}
		assert(upstream.GetInfo().m_TotalSize == 0);
		assert(upstream.GetInfo().m_NumAllocs == 0);
		{
			TPool pool{ 1, &upstream };
			PolymorphicMemoryPool poly{ std::move(pool) };
			assert(poly.IsValid());
			pool.Reset();
			assert(upstream.GetInfo().m_NumAllocs == 1);

			void* ptr = poly.Allocate(1);
			assert(ptr);
			assert(upstream.GetInfo().m_NumAllocs == 1);

			poly.Deallocate(ptr, 1);
		}
	}
} // namespace brk::memory::ut