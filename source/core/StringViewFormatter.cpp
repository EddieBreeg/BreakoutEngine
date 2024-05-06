#include "StringViewFormatter.hpp"

#include <fmt/std.h>
#include <string_view>

fmt::appender fmt::formatter<brk::StringView>::format(
	const brk::StringView view,
	format_context& ctx) const
{
	string_view tmp;
	if (view.GetPtr())
	{
		tmp = { view.GetPtr(), view.GetLen() };
	}
	return formatter<string_view>::format(tmp, ctx);
}
