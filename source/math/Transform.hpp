#pragma once

#include "Matrix.hpp"
#include "Quaternion.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace math {
	struct Transform
	{
		float3 m_Translate = float3{ 0 };
		float3 m_Scale = { 1, 1, 1 };
		Quaternion m_Rotation = glm::identity<Quaternion>();

		[[nodiscard]] constexpr Matrix44 GetMatrix() const noexcept
		{
			const float4x4 rotation = glm::mat4_cast(m_Rotation);
			return glm::translate(glm::scale(rotation, m_Scale), m_Translate);
		}
	};
} // namespace math