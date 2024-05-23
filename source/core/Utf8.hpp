#pragma once

#include <PCH.hpp>
#include "StringView.hpp"
#include <optional>

namespace brk::utf8 {
	/**
	 * Attempts to convert a string into an integer, using the provided base.
	 * \tparam Int The integer type into
	 * \param str String string to parse
	 * \return If the string does indeed represent a valid integral value in the provided
	 * base, this value is returned. Otherwise, an empty optional is returned instead
	 */
	template <
		class Int,
		uint8 Base = 10,
		std::enable_if_t<std::is_integral_v<Int>, int> = 0>
	constexpr std::optional<Int> FromString(StringView str) noexcept;

	template <class Int, std::enable_if_t<std::is_integral_v<Int>, int> = 0>
	constexpr Int FromChar(const char c) noexcept;
} // namespace brk::utf8

#include "Utf8.inl"