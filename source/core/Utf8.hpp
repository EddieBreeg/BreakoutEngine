#pragma once

#include <PCH.hpp>
#include <string_view>
#include <optional>

namespace brk::utf8 {
	template <
		class Int,
		uint8 Base = 10,
		std::enable_if_t<std::is_integral_v<Int>, int> = 0>
	constexpr std::optional<Int> FromString(const std::string_view str) noexcept;

	template <class Int, std::enable_if_t<std::is_integral_v<Int>, int> = 0>
	constexpr Int FromChar(const char c) noexcept;
} // namespace brk::utf8

#include "Utf8.inl"