#pragma once

#include <PCH.hpp>
#include <string_view>

namespace brk {
	class ULID
	{
	public:
		constexpr ULID() noexcept = default;
		~ULID() = default;

		constexpr ULID(
			const uint64 timestamp,
			const uint16 rand1,
			const uint64 rand2) noexcept;

		[[nodiscard]] static ULID Generate();

		template <uint32 N>
		constexpr char* ToString(char (&out_buf)[N]) const noexcept;

		[[nodiscard]] static constexpr ULID FromString(
			const std::string_view str) noexcept;

		[[nodiscard]] constexpr operator bool() const noexcept
		{
			return m_Left && m_Right;
		}

		[[nodiscard]] constexpr bool operator==(const ULID other) const noexcept;

	private:
		uint64 m_Left = 0, m_Right = 0;
	};
} // namespace brk

#include "ULID.inl"