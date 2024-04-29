#include <core/Utf8.hpp>

namespace brk::utf8::ut {
	void Utf8Tests()
	{
		static_assert(FromChar<int>('1') == 1);
		static_assert(FromChar<int>('2') == 2);
		static_assert(FromChar<int>('a') == 10);
		static_assert(FromChar<int>('A') == 10);
		static_assert(FromChar<int>('B') == 11);
		static_assert(FromChar<int>(' ') == -1);
		static_assert(FromChar<int>('-') == -1);

		static_assert(FromString<int>("10") == 10);
		static_assert(FromString<int>("123") == 123);
		static_assert(FromString<int, 16>("ff") == 255);
		static_assert(!FromString<int, 16>(" ff").has_value());
	}
} // namespace brk::utf8::ut
