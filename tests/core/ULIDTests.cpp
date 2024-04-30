#include <iostream>
#include <core/ULID.hpp>
#include <nlohmann/json.hpp>
#include <cassert>
#include <array>

namespace brk::ulid::ut {
	void ULIDTests()
	{
		{
			constexpr brk::ULID id;
			static_assert(!id);

			char out[26];
			id.ToChars(out);
			for (const char x : out)
				assert(x == '0');
		}
		{
			const brk::ULID id = brk::ULID::Generate();
			char str[27] = { 0 };
			id.ToChars(str);
			std::cout << str << '\n';
			const brk::ULID id2 = brk::ULID::FromString(str);
			assert(id == id2);
		}
		{
			static_assert(
				brk::ULID::FromString("01HWPC5Y8GZZ3A6AQVX4PYRXHC") ==
				brk::ULID{ 0x018f2cc2f910, 0xffc6, 0xa32afbe92dec762c });
		}
		{
			const nlohmann::json data = "01HWPC5Y8GZZ3A6AQVX4PYRXHC";
			const brk::ULID id = data;
			assert((id == brk::ULID{ 0x018f2cc2f910, 0xffc6, 0xa32afbe92dec762c }));
		}
		{
			constexpr brk::ULID id{ 0x018f2cc2f910, 0xffc6, 0xa32afbe92dec762c };
			const nlohmann::json j = id;
			assert(j.is_string() && j == "01HWPC5Y8GZZ3A6AQVX4PYRXHC");
		}
	}
} // namespace brk::ulid::ut
