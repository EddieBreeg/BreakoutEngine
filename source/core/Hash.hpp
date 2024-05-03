#pragma once

#include <PCH.hpp>

namespace brk {
	template <class T, class = void>
	struct Hash;

	namespace _internal {
		template <class T, class = void>
		struct HashInfo;

		template <class T>
		struct HashInfo<
			T,
			std::enable_if_t<
				std::is_unsigned_v<std::invoke_result_t<const Hash<T>, const T>>>>
		{
			using Type = std::invoke_result_t<const Hash<T>, const T>;
			static constexpr bool IsNoThrow =
				noexcept(std::declval<Hash<T>>()(std::declval<const T>()));
		};
	} // namespace _internal

	template <class T>
	static constexpr bool IsHashable = IsComplete<_internal::HashInfo<T>>;

	template <class T>
	static constexpr bool IsNoThrowHashable =
		IsHashable<T> && _internal::HashInfo<T>::IsNoThrow;

	template <class Int>
	struct Hash<Int, std::enable_if_t<std::is_integral_v<Int>>>
	{
		[[nodiscard]] constexpr auto operator()(const Int x) const noexcept
			-> std::make_unsigned_t<Int>;
	};

	template <class T>
	[[nodiscard]] constexpr uint64 CombineHash(uint64 h, T&& val) noexcept(
		IsNoThrowHashable<std::decay_t<T>>);
} // namespace brk

#include "Hash.inl"