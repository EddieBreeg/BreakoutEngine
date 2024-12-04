#include "TransformComponent.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

float4x4 brk::TransformComponent::GetMatrix() const
{
	return glm::scale(
		glm::translate(glm::identity<float4x4>(), m_Translate) *
			glm::mat4_cast(m_Rotation),
		m_Scale);
}