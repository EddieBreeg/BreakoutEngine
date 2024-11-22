#include <core/RetainPtr.hpp>
#include <cassert>

namespace brk::retain_ptr::ut {
	template <bool Atomic = false>
	struct Ref1 : public BasicRefCount<Atomic>
	{
		int32& count;
		Ref1(int32& val)
			: count{ ++val }
		{}
		~Ref1() { --count; }
	};

	template <bool Atomic>
	void TraitsTests()
	{
		using TRef = Ref1<Atomic>;
		using Traits = RetainTraits<TRef>;
		int32 n = 0;
		TRef* ptr = new TRef{ n };
		assert(n == 1);

		assert(Traits::GetCount(ptr) == 1);
		Traits::Increment(ptr);
		assert(Traits::GetCount(ptr) == 2);
		Traits::Decrement(ptr);
		assert(Traits::GetCount(ptr) == 1);
		Traits::Decrement(ptr);
		assert(n == 0);
	}

	void Tests()
	{
		static_assert(
			std::is_same_v<_internal::RefCountBase<Ref1<false>>::BaseType, TRefCount>);
		static_assert(std::is_same_v<
					  _internal::RefCountBase<Ref1<true>>::BaseType,
					  TAtomicRefCount>);
		static_assert(!meta::IsComplete<_internal::RefCountBase<void>>);

		TraitsTests<false>();
		TraitsTests<true>();

		{
			RetainPtr<Ref1<false>> ptr;
			assert(ptr.Get() == nullptr);
			assert(ptr.GetCount() == 0);
		}
		int32 n = 0;
		{
			auto* ref = new Ref1<false>{ n };
			assert(n == 1);
			RetainPtr ptr{ ref, s_Adopt };
			assert(ptr.GetCount() == 1);
		}
		assert(n == 0);
		{
			auto* ref = new Ref1<false>{ n };
			assert(n == 1);
			{
				RetainPtr ptr{ ref, s_Retain };
				assert(ptr.GetCount() == 2);
			}
			assert(RetainTraits<Ref1<false>>::GetCount(ref) == 1);
			delete ref;
		}
		assert(n == 0);
		{
			auto* ref = new Ref1<false>{ n };
			assert(n == 1);
			RetainPtr ptr{ ref };
			assert(ptr.GetCount() == 1);
		}
		assert(n == 0);
		{
			auto* ref = new Ref1<false>{ n };
			assert(n == 1);
			RetainPtr ptr{ ref };
			ptr.Reset(nullptr, s_Adopt);
			assert(!ptr.Get());
			assert(n == 0);
		}
		{
			RetainPtr ptr{ new Ref1<false>{ n } };
			assert(n == 1 && ptr.GetCount() == 1);
			auto* p2 = new Ref1<false>{ n };
			assert(n == 2);
			ptr.Reset(p2, s_Retain);
			assert(n == 1);
			assert(ptr.GetCount() == 2);
			ptr.Reset();
			assert(ptr.Get() == nullptr);
			delete p2;
		}
		{
			RetainPtr ptr{ new Ref1<false>{ n } };
			assert(n == 1);
			{
				RetainPtr ptr2{ ptr };
				assert(ptr.GetCount() == 2);
				assert(ptr2.GetCount() == 2);
			}
		}
		assert(n == 0);
		{
			RetainPtr ptr{ new Ref1<false>{ n } };
			assert(n == 1);
			{
				RetainPtr ptr2{ std::move(ptr) };
				assert(ptr.GetCount() == 0);
				assert(ptr2.GetCount() == 1);
			}
			assert(n == 0);
		}
		{
			RetainPtr ptr{ new Ref1<false>{ n } };
			{
				RetainPtr ptr2{ new Ref1<false>{ n } };
				ptr = ptr2;
				assert(ptr.Get() == ptr2.Get());
				assert(ptr.GetCount() == 2);
				assert(ptr2.GetCount() == 2);
			}
			assert(ptr.GetCount() == 1);
		}
		assert(n == 0);
		{
			RetainPtr ptr{ new Ref1<false>{ n } };
			{
				RetainPtr ptr2{ new Ref1<false>{ n } };
				ptr = std::move(ptr2);
				assert(ptr.GetCount() == 1);
			}
			assert(n == 1);
			assert(ptr.GetCount() == 1);
		}
		assert(n == 0);
		{
			RetainPtr ptr{ new Ref1<false>{ n } };
			auto* p = ptr.Release();
			assert(ptr.Get() == nullptr);
			assert(n == 1);
			delete p;
		}
	}
} // namespace brk::retain_ptr::ut
