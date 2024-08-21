#pragma once

#include <PCH.hpp>
#include <core/StringView.hpp>
#include <core/FieldList.hpp>

namespace brk {
	struct TransformComponent
	{
		float3 m_Translate;

		static constexpr StringView Name = "transform";
		static constexpr meta::FieldList<&TransformComponent::m_Translate> Fields{
			"translate",
		};
	};

} // namespace brk
