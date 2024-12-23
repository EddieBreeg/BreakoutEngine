#include "TransformComponent.hpp"
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <ecs/Component.hpp>
#include <editor/brk_editor_export.h>
#include <managers/ResourceManager.hpp>
#include <math/Constants.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <imgui.h>

template <>
struct brk::ecs::ComponentUiWidget<brk::TransformComponent>
{
	ComponentUiWidget(const TransformComponent& transform)
		: m_Translate{ transform.m_Translate }
		, m_Scale{ transform.m_Scale }
		, m_EulerRotation{ glm::eulerAngles(transform.m_Rotation) }
	{}
	bool operator()(TransformComponent& comp)
	{
		bool res = ImGui::DragFloat3("Translate", &m_Translate.x);
		res |= ImGui::DragFloat3("Rotation", &m_EulerRotation.x);
		res |= ImGui::DragFloat3("Scale", &m_Scale.x);
		comp.m_Translate = m_Translate;
		comp.m_Scale = m_Scale;
		comp.m_Rotation = { (math::Pi / 180.f) * m_EulerRotation };
		return res;
	}

	float3 m_Translate, m_Scale, m_EulerRotation;
};

const brk::ecs::ComponentInfo brk::TransformComponent::Info =
	brk::ecs::ComponentInfo::Create<brk::TransformComponent>("transform");

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