#pragma once

#include <PCH.hpp>

namespace brk::meta {
	/**
	 * Represents a list of member fields, used for serialization/deserialization purposes
	 * \tparam Fields Pointers to the member objects
	 */
	template <auto... Fields>
	struct FieldList
	{
		static constexpr size_t Count = sizeof...(Fields);
		/**
		 * The combined size of all fields
		 */
		static constexpr size_t TotalSize = (sizeof(decltype(Fields)) + ...);

		static_assert(
			(std::is_member_object_pointer_v<decltype(Fields)> && ...),
			"Fields must be pointers to member objects");

		/**
		 * The fields' names, typically used for json conversions
		 */
		const char* const m_Names[sizeof...(Fields)] = { nullptr };
	};
} // namespace brk::meta
