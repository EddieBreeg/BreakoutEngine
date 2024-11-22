#pragma once

#include <PCH.hpp>

namespace brk {
	/**
	 * Generic hash function API
	 */
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

	/**
	 * This value is true for types T such that, for h an object of type const Hash<T>
	 * and x an object of type const T&, h(x) is a valid expression which returns an
	 * unsigned integer
	 */
	template <class T>
	static constexpr bool IsHashable = meta::IsComplete<_internal::HashInfo<T>>;

	template <class T>
	static constexpr bool IsNoThrowHashable =
		IsHashable<T> && _internal::HashInfo<T>::IsNoThrow;

	/**
	 * Specialization of Hash for integer types
	 */
	template <class Int>
	struct Hash<Int, std::enable_if_t<std::is_integral_v<Int>>>
	{
		[[nodiscard]] constexpr auto operator()(const Int x) const noexcept
			-> std::make_unsigned_t<Int>;
	};

	/**
	 * Utility function which hashes an object, and combines the result with a pre
	 * existing hash. This can be used to hash more complicated structures like tuples and
	 * arrays
	 * \param h: The hash to be combined with the new value
	 * \param val: The object to be hashed. The type of val must meet the requirements of
	 * IsHashable
	 */
	template <class T>
	[[nodiscard]] constexpr uint64 CombineHash(uint64 h, T&& val) noexcept(
		IsNoThrowHashable<std::decay_t<T>>);
} // namespace brk

#include "Hash.inl"