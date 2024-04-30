#include <iostream>
#include <core/ULID.hpp>
#include <cassert>
#include <array>

namespace brk::ulid::ut {
	void ULIDTests()
	{
		{
			constexpr brk::ULID id;
			static_assert(!id);

			char out[26];
			id.ToString(out);
			for (const char x : out)
				assert(x == '0');
		}
		{
			const brk::ULID id = brk::ULID::Generate();
			char str[27] = { 0 };
			id.ToString(str);
			std::cout << str << '\n';
			const brk::ULID id2 = brk::ULID::FromString(str);
			assert(id == id2);
		}
		{
			static_assert(
				brk::ULID::FromString("01HWPC5Y8GZZ3A6AQVX4PYRXHC") ==
				brk::ULID{ 0x018f2cc2f910, 0xffc6, 0xa32afbe92dec762c });
		}
	}
} // namespace brk::ulid::ut
