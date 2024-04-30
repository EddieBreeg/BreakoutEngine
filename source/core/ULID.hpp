#pragma once

#include <PCH.hpp>

#include "FieldList.hpp"

#include <nlohmann/json_fwd.hpp>
#include <string_view>

namespace brk {
	template <class, class>
	struct BinaryLoader;

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
		constexpr char* ToChars(char (&out_buf)[N]) const noexcept;

		[[nodiscard]] static constexpr ULID FromString(
			const std::string_view str) noexcept;

		[[nodiscard]] constexpr operator bool() const noexcept
		{
			return m_Left && m_Right;
		}

		[[nodiscard]] constexpr bool operator==(const ULID other) const noexcept;

	private:
		uint64 m_Left = 0, m_Right = 0;

		static constexpr meta::FieldList<&ULID::m_Left, &ULID::m_Right> Fields{};

		friend struct BinaryLoader<ULID, void>;
	};

	void from_json(const nlohmann::json& out_json, ULID& id);
	void to_json(nlohmann::json& out_json, const ULID& id);

} // namespace brk

#include "ULID.inl"