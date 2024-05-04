#include <core/Function.hpp>
#include <cassert>

namespace brk::unique_function::ut {
	struct Foo
	{
		static int n;
		bool moved = false;
		Foo() { ++n; }
		Foo(Foo&& other) { other.moved = true; }

		void operator()() {}

		~Foo()
		{
			if (!moved)
				--n;
		}
	};
	int Foo::n = 0;

	struct Bar
	{
		int x = 0;
		bool* moved = nullptr;
		Bar() = default;
		Bar(int val, bool* moved = nullptr)
			: x{ val }
			, moved{ moved }
		{}
		int operator()() const { return x; }
		Bar(const Bar&) = delete;
		Bar(Bar&& other)
			: x{ other.x }
			, moved(other.moved)
		{}
		Bar& operator=(const Bar&) = delete;
		Bar& operator=(Bar&& other)
		{
			x = other.x;
			*other.moved = true;
			return *this;
		}
	};

	void Tests()
	{
		{
			const UniqueFunction<void()> f;
			assert(!f);
			assert(f.GetId().m_Index == brk::TypeId::Get<void>().m_Index);
		}
		{
			const UniqueFunction<void()> f = Foo{};
			assert(f);
			assert(Foo::n == 1);
		}
		assert(Foo::n == 0);
		{
			UniqueFunction<void()> f1 = Foo{};
			assert(Foo::n == 1);
			UniqueFunction<void()> f2 = std::move(f1);
			assert(Foo::n == 1);
			assert(!f1);
			assert(f2);
		}
		{
			int result = 0;
			UniqueFunction<void()> f1 = [&]()
			{
				++result;
			};
			f1();
			assert(result == 1);
			UniqueFunction<void()> f2 = std::move(f1);
			f2();
			assert(result == 2);
		}
		{
			int result = 0;
			UniqueFunction<void()> f;
			f = [&]()
			{
				++result;
			};
			f();
			assert(result == 1);
		}
		{
			bool moved = false;
			UniqueFunction<int()> f{ Bar{ 0 } };
			f = Bar{ 1, &moved };
			assert(moved);
			assert(f() == 1);
		}
		{
			UniqueFunction<int()> f = []()
			{
				return 0;
			};
			bool moved = false;
			f = Bar{ 1, &moved };
			// f doesn't contain an object of type Bar: moved assignment won't be called
			assert(!moved);
			assert(f() == 1);
		}
	}
} // namespace brk::unique_function::ut
