#pragma once

#include <type_traits>

namespace brk {
	template <class T, class... U>
	constexpr auto Max(T&& left, U&&... right) noexcept
	{
		static_assert(
			(std::is_same_v<std::decay_t<T>, std::decay_t<U>> && ...),
			"All values must have the same type");
		T res = std::forward<T>(left);

		return ((res = res > right ? res : std::forward<U>(right)), ...);
	}

	template <class T, class... U>
	constexpr auto Min(T&& left, U&&... right) noexcept
	{
		static_assert(
			(std::is_same_v<std::decay_t<T>, std::decay_t<U>> && ...),
			"All values must have the same type");
		T res = std::forward<T>(left);

		return ((res = res < right ? res : std::forward<U>(right)), ...);
	}

	template <class T, size_t N>
	[[nodiscard]] constexpr size_t ArraySize(const T (&arr)[N]) noexcept
	{
		return N;
	}
} // namespace brk
