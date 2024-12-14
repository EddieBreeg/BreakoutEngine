#pragma once

#include <PCH.hpp>

namespace brk {
	template <class E>
	[[nodiscard]] constexpr auto ToUnderlying(E val) noexcept
	{
		static_assert(std::is_enum_v<E>, "E must be an enum type");
		return static_cast<std::underlying_type_t<E>>(val);
	}

	template <class E>
	struct EnumFlags
	{
		static_assert(std::is_enum_v<E>, "E must be an enum type");

		constexpr EnumFlags() = default;
		constexpr EnumFlags(E flags)
			: m_Flags{ ToInt(flags) }
		{}

		constexpr EnumFlags(const EnumFlags&) = default;
		constexpr EnumFlags& operator=(const EnumFlags&) = default;

		constexpr EnumFlags& operator=(E flags) noexcept;

		[[nodiscard]] constexpr E Get() const noexcept;
		constexpr void Set(E flags) noexcept;
		constexpr void Clear(E flags) noexcept;
		constexpr void Flip(E flags) noexcept;

		constexpr void SetAll() noexcept;
		constexpr void ClearAll() noexcept;

		[[nodiscard]] constexpr bool HasAny(E flags) const noexcept;
		[[nodiscard]] constexpr bool HasAll(E flags) const noexcept;

		[[nodiscard]] constexpr bool operator==(const EnumFlags other) const noexcept;
		[[nodiscard]] constexpr bool operator!=(const EnumFlags other) const noexcept;

	private:
		using ValueType = std::make_unsigned_t<std::underlying_type_t<E>>;
		static constexpr ValueType ToInt(E val) noexcept
		{
			return static_cast<ValueType>(val);
		}

		ValueType m_Flags = 0;
	};
} // namespace brk

template <class E, std::enable_if_t<std::is_enum_v<E>>* = nullptr>
constexpr E operator|(E a, E b) noexcept
{
	return static_cast<E>(brk::ToUnderlying(a) | brk::ToUnderlying(b));
}

#include "EnumFlags.inl"