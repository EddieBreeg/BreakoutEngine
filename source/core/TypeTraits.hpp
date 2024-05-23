#pragma once

#include <PCH.hpp>

namespace brk {
	template <class Char>
	struct BasicStringView;
} // namespace brk

namespace brk::meta {
	namespace _internal {
		template <class T, class = void>
		struct IsComplete : std::false_type
		{};

		template <class T>
		struct IsComplete<T, std::enable_if_t<sizeof(T)>> : std::true_type
		{};

		template <class T, class = void>
		struct HasName : std::false_type
		{};
		template <class T>
		struct HasName<
			T,
			std::enable_if_t<
				std::is_same_v<decltype(T::Name), const BasicStringView<char>>>>
			: std::true_type
		{};
	} // namespace _internal

	/* Tests whether T is a complete type */
	template <class T>
	static constexpr bool IsComplete = _internal::IsComplete<T>::value;

	/**
	 * Tests whether T has an accessible static StringView called Name
	 */
	template <class T>
	static constexpr bool HasName = _internal::HasName<T>::value;
} // namespace brk::meta
