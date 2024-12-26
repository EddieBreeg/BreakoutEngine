#include "TransformComponent.hpp"
#include <core/Loaders.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

float4x4 brk::TransformComponent::GetMatrix() const
{
	// clang-format off
	float4x4 transform = glm::mat4_cast(m_Rotation) *  float4x4 {
		m_Scale.x, 0, 0, 0, 
		0, m_Scale.y, 0, 0,
		0, 0, m_Scale.z, 0,
		0, 0, 0, 1,
	};
	// clang-format on
	transform[3] = float4{ m_Translate, 1.0f };
	return transform;
}