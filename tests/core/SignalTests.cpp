#include <core/Signal.hpp>
#include <cassert>

namespace brk::signal::ut {
	void Tests()
	{
		{
			int res = 0;
			Signal<void(int)> s;
			s += [&](int x)
			{
				res += x;
			};
			s += [&](int x)
			{
				res -= x / 2;
			};

			s.Emit(2);
			assert(res == 1);
		}
		{
			int res = 0;
			Signal<int(int)> s;
			s += [](int x)
			{
				return x;
			};
			s += [](int x)
			{
				return x * 2;
			};

			s.Emit(
				[&res](int x)
				{
					res += x;
				},
				1);
			assert(res == 3);
		}
	}
} // namespace brk::signal::ut
