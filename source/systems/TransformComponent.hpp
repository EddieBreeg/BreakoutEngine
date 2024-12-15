#pragma once

#include <PCH.hpp>
#include <core/StringView.hpp>
#include <core/FieldList.hpp>
#include <ecs/ComponentFwd.hpp>

#include <math/Transform.hpp>

namespace brk {
	struct BRK_SYSTEMS_API TransformComponent
	{
		float3 m_Translate = { 0, 0, 0 };
		float3 m_Scale = { 1, 1, 1 };
		math::Quaternion m_Rotation = glm::identity<math::Quaternion>();

		[[nodiscard]] float4x4 GetMatrix() const;

		static const ecs::ComponentInfo Info;
		static constexpr meta::FieldList<
			&TransformComponent::m_Translate,
			&TransformComponent::m_Scale,
			&TransformComponent::m_Rotation>
			Fields{
				"translate",
				"scale",
				"rotation",
			};
	};

} // namespace brk
