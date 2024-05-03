#pragma once

#include <PCH.hpp>

#include "FieldList.hpp"
#include "Hash.hpp"

#include <nlohmann/json_fwd.hpp>
#include <string_view>

namespace std {
	template <class T>
	class allocator;

	template <class Key>
	struct equal_to;

	template <class Key, class T, class Hash, class KeyEqual, class Allocator>
	class unordered_map;
} // namespace std

namespace brk {
	template <class, class>
	struct BinaryLoader;

	class ULID
	{
	public:
		constexpr ULID() noexcept = default;
		constexpr ULID(const ULID&) = default;
		~ULID() = default;
		ULID& operator=(const ULID&) = default;

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
		friend struct Hash<ULID>;
	};

	void from_json(const nlohmann::json& out_json, ULID& id);
	void to_json(nlohmann::json& out_json, const ULID& id);

	template <>
	struct Hash<ULID>
	{
		[[nodiscard]] constexpr uint64 operator()(const ULID id) const noexcept;
	};

	template <class T>
	using TULIDMap = std::unordered_map<
		ULID,
		T,
		Hash<ULID>,
		std::equal_to<ULID>,
		std::allocator<std::pair<const ULID, T>>>;
} // namespace brk

#include "ULID.inl"