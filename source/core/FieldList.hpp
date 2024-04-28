#pragma once

#include <PCH.hpp>

namespace brk::meta {
	template <auto... Fields>
	struct FieldList
	{
		static constexpr size_t Count = sizeof...(Fields);
		static constexpr size_t TotalSize = (sizeof(decltype(Fields)) + ...);

		static_assert(
			(std::is_member_object_pointer_v<decltype(Fields)> && ...),
			"Fields must be pointers to member objects");

		const char* const m_Names[sizeof...(Fields)] = {};
	};
} // namespace brk::meta
