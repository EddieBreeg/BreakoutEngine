#include "EnumFlags.hpp"
namespace brk {
	template <class E>
	inline constexpr EnumFlags<E>& EnumFlags<E>::operator=(E flags) noexcept
	{
		m_Flags = ToInt(flags);
		return *this;
	}

	template <class E>
	inline constexpr E EnumFlags<E>::Get() const noexcept
	{
		return static_cast<E>(m_Flags);
	}

	template <class E>
	inline constexpr void EnumFlags<E>::Set(E flags) noexcept
	{
		m_Flags |= ToInt(flags);
	}

	template <class E>
	inline constexpr void EnumFlags<E>::Clear(E flags) noexcept
	{
		m_Flags &= ~ToInt(flags);
	}

	template <class E>
	inline constexpr void EnumFlags<E>::Flip(E flags) noexcept
	{
		m_Flags ^= ToInt(flags);
	}

	template <class E>
	inline constexpr void EnumFlags<E>::SetAll() noexcept
	{
		m_Flags = ~ValueType(0);
	}

	template <class E>
	inline constexpr void EnumFlags<E>::ClearAll() noexcept
	{
		m_Flags = ValueType(0);
	}

	template <class E>
	inline constexpr bool EnumFlags<E>::HasAny(E flags) const noexcept
	{
		return m_Flags & ToInt(flags);
	}

	template <class E>
	inline constexpr bool EnumFlags<E>::HasAll(E flags) const noexcept
	{
		return (m_Flags & ToInt(flags)) == ToInt(flags);
	}

	template <class E>
	inline constexpr bool EnumFlags<E>::operator==(const EnumFlags other) const noexcept
	{
		return m_Flags == other.m_Flags;
	}

	template <class E>
	inline constexpr bool EnumFlags<E>::operator!=(const EnumFlags other) const noexcept
	{
		return m_Flags != other.m_Flags;
	}
} // namespace brk