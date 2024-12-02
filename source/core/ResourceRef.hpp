#pragma once

#include "Resource.hpp"
#include "RetainPtr.hpp"

namespace brk {
	template <>
	struct BRK_CORE_API RetainTraits<Resource>
	{
		static constexpr Retain_t DefaultAction = {};
		static void Increment(Resource* res);
		static void Decrement(Resource* res);
		static uint32 GetCount(const Resource* res);
	};

	template <class Res>
	using ResourceRef = RetainPtr<Res, RetainTraits<Resource>>;
} // namespace brk
