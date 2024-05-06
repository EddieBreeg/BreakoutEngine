#include "ULIDFormatter.hpp"

fmt::appender fmt::formatter<brk::ULID>::format(const brk::ULID id, format_context& ctx)
	const
{
	using Base = formatter<char[26]>;
	char str[26];
	id.ToChars(str);
	return formatter<basic_string_view<char>>::format(
		basic_string_view<char>{ str, 26 },
		ctx);
}
