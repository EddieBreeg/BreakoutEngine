#pragma once

#include <PCH.hpp>
#include <fmt/core.h>

namespace brk {
	class Resource;
} // namespace brk

template <>
struct BRK_CORE_API fmt::formatter<brk::Resource>
{
	constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator
	{
		return ctx.begin();
	}

	fmt::appender format(const brk::Resource& res, format_context& ctx);
};