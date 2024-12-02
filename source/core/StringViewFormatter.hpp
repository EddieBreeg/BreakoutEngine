#pragma once

#include <PCH.hpp>
#include "StringView.hpp"
#include <fmt/core.h>

template <>
struct BRK_CORE_API fmt::formatter<brk::StringView> : fmt::formatter<fmt::string_view>
{
	appender format(const brk::StringView view, format_context& ctx) const;
};