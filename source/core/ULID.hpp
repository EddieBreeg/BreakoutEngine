#pragma once

#include <PCH.hpp>

#include "FieldList.hpp"
#include "Hash.hpp"

#include <nlohmann/json_fwd.hpp>
#include "StringView.hpp"

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

	/**
	 * Unique lexographically sortable identifier. The spec for this format can
	 * be found here: https://github.com/ulid/spec
	 */
	class ULID
	{
	public:
		/**
		 * Creates a null identifier (all 0s)
		 */
		constexpr ULID() noexcept = default;
		constexpr ULID(const ULID&) = default;
		~ULID() = default;
		ULID& operator=(const ULID&) = default;

		/**
		 * Constructs a ULID from a timestamp and random values
		 */
		constexpr ULID(
			const uint64 timestamp,
			const uint16 rand1,
			const uint64 rand2) noexcept;

		/**
		 * Generates a new identifier, with a 48-bit unix timestamp (in milliseconds) and
		 * 80 bits of pseudo-random data
		 */
		[[nodiscard]] static ULID Generate();

		/**
		 * Converts the ULID object into a base 32 string
		 */
		template <uint32 N>
		constexpr char* ToChars(char (&out_buf)[N]) const noexcept;

		/**
		 * Attempts to create a ULID object from a base 32 string. If the string doesn't
		 * represent a valid ULID object, a null id is returned instead
		 */
		[[nodiscard]] static constexpr ULID FromString(const StringView str) noexcept;

		/**
		 * Tests whether this id is non-null
		 */
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

	void from_json(const nlohmann::json& json, ULID& out_id);
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