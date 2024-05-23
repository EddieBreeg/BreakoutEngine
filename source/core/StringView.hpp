#pragma once

#include <PCH.hpp>
#include "Assert.hpp"
#include <nlohmann/json_fwd.hpp>

namespace brk {
	template<class T, class>
	struct Hash;

	template <class Char>
	struct BasicStringView
	{
		constexpr BasicStringView() noexcept = default;
		constexpr BasicStringView(const Char* str, const uint32 len);
		constexpr BasicStringView(const Char* str);
		constexpr BasicStringView(const std::string& str);
		constexpr BasicStringView(const BasicStringView&) = default;
		constexpr BasicStringView& operator=(const BasicStringView&) = default;

		constexpr void TrimLeft(const uint32 n) noexcept;

		[[nodiscard]] constexpr bool IsEmpty() const noexcept { return !m_Len; }

		[[nodiscard]] constexpr const Char* GetPtr() const noexcept { return m_Ptr; }
		[[nodiscard]] constexpr uint32 GetLen() const noexcept { return m_Len; }

		[[nodiscard]] constexpr const Char* begin() const noexcept { return m_Ptr; }
		[[nodiscard]] constexpr const Char* end() const noexcept { return m_Ptr + m_Len; }

		[[nodiscard]] Char operator[](const uint32 index) const;

	private:
		const Char* m_Ptr = nullptr;
		uint32 m_Len = 0;
	};

	using StringView = BasicStringView<char>;

	template <>
	struct Hash<StringView, void>
	{
		[[nodiscard]] constexpr uint32 operator()(const StringView str) const noexcept;
	};

	void from_json(const nlohmann::json& json, StringView& str);
	void to_json(nlohmann::json& json, const StringView& str);
} // namespace brk

#include "StringView.inl"