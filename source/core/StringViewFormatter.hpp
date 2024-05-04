#pragma once

#include <PCH.hpp>
#include "StringView.hpp"
#include <string_view>
#include <fmt/core.h>
#include <fmt/std.h>

template <>
struct fmt::formatter<brk::StringView> : public fmt::formatter<std::string_view>
{
	auto format(const brk::StringView view, format_context& ctx) const
	{
		std::string_view tmp;
		if (view.GetPtr())
		{
			tmp = { view.GetPtr(), view.GetLen() };
		}

		return formatter<std::string_view>::format(tmp, ctx);
	}
};