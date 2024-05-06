#pragma once

#include <PCH.hpp>
#include "ULID.hpp"
#include <fmt/core.h>

template<>
struct fmt::formatter<brk::ULID>: public fmt::formatter<fmt::basic_string_view<char>>
{
	fmt::appender format(const brk::ULID id, format_context& ctx) const;
};