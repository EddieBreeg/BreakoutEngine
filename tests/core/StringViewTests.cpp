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

			static_assert(str.Find('f') == 0);
			static_assert(str.Find('o') == 1);
			static_assert(str.Find('b') == 3);
			static_assert(str.Find("foo") == 0);
			static_assert(str.Find("bar") == 3);
			static_assert(str.Find('n') == str.NPos);
			static_assert(str.Find("blabla") == str.NPos);
		}
	}
} // namespace brk::string_view::ut
