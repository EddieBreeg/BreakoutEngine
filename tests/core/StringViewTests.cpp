#include <core/StringView.hpp>
#include <cassert>

namespace brk::string_view::ut
{
	void Tests()
	{
		{
			constexpr BasicStringView str{ "foobar" };
			static_assert(str.GetLen() == 6);
			static_assert((str.end() - str.begin()) == str.GetLen());
			assert(str[3] == 'b');
		}
	}
} // namespace brk::string_view::ut
