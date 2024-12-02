#pragma once

#include <PCH.hpp>
#include "Assert.hpp"
#include <nlohmann/json_fwd.hpp>

namespace brk {
	template <class T, class>
	struct Hash;

	/**
	 * Read-only span over a range of characters
	 */
	template <class Char>
	struct BasicStringView
	{
		/** Empty view. Calling GetPtr() on the created object returns nullptr */
		constexpr BasicStringView() noexcept = default;
		constexpr BasicStringView(const Char* str, const uint32 len);
		constexpr BasicStringView(const Char* str);
		constexpr BasicStringView(const std::string& str);
		constexpr BasicStringView(const BasicStringView&) = default;
		constexpr BasicStringView& operator=(const BasicStringView&) = default;

		[[nodiscard]] constexpr bool operator==(const BasicStringView& other);
		[[nodiscard]] constexpr bool operator!=(const BasicStringView& other);

		static constexpr uint32 NPos = uint32(-1);

		/**
		 * Deletes the first characters of the view
		 * \param n: The number of characters to trim. If n >= GetLen(), the internal
		 * pointer is set to nullptr and the length is 0
		 */
		constexpr void TrimLeft(const uint32 n) noexcept;

		[[nodiscard]] constexpr bool IsEmpty() const noexcept { return !m_Len; }

		[[nodiscard]] constexpr const Char* GetPtr() const noexcept { return m_Ptr; }
		[[nodiscard]] constexpr uint32 GetLen() const noexcept { return m_Len; }

		[[nodiscard]] constexpr uint32 Find(Char c) const noexcept;
		[[nodiscard]] constexpr uint32 Find(BasicStringView sub) const noexcept;

		[[nodiscard]] constexpr const Char* begin() const noexcept { return m_Ptr; }
		[[nodiscard]] constexpr const Char* end() const noexcept { return m_Ptr + m_Len; }

		[[nodiscard]] Char operator[](const uint32 index) const;

	private:
		const Char* m_Ptr = nullptr;
		uint32 m_Len = 0;
	};

	using StringView = BasicStringView<char>;

	/**
	 * Specialized hash algorithm for string views. Uses a modified version of the
	 * Jenkins-One-At-A-Time algorithm
	 */
	template <>
	struct Hash<StringView, void>
	{
		[[nodiscard]] constexpr uint32 operator()(const StringView str) const noexcept;
	};

	BRK_CORE_API void from_json(const nlohmann::json& json, StringView& str);
	BRK_CORE_API void to_json(nlohmann::json& json, const StringView& str);
} // namespace brk

#include "StringView.inl"