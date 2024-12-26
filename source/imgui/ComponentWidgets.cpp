#include "ComponentWidgets.hpp"
#include "MiscWidgets.hpp"

#include <glm/gtc/quaternion.hpp>
#include <imgui.h>
#include <systems/TransformComponent.hpp>
#include <systems/VisualComponents.hpp>

namespace brk::ecs {
	ComponentUiWidget<brk::TransformComponent>::ComponentUiWidget(
		const TransformComponent& transform)
		: m_Translate{ transform.m_Translate }
		, m_Scale{ transform.m_Scale }
		, m_EulerRotation{ glm::eulerAngles(transform.m_Rotation) }
	{}

	bool ComponentUiWidget<brk::TransformComponent>::operator()(TransformComponent& comp)
	{
		bool res = ImGui::DragFloat3("Translate", &m_Translate.x);
		res |= ImGui::DragFloat3("Rotation", &m_EulerRotation.x);
		res |= ImGui::DragFloat3("Scale", &m_Scale.x);
		comp.m_Translate = m_Translate;
		comp.m_Scale = m_Scale;
		comp.m_Rotation = { (math::Pi / 180.f) * m_EulerRotation };
		return res;
	}

	ecs::ComponentUiWidget<brk::MeshComponent>::ComponentUiWidget(const MeshComponent&) {}

	bool ComponentUiWidget<brk::MeshComponent>::operator()(MeshComponent& mesh)
	{
		dev_ui::ULIDWidget("Mesh", mesh.m_MeshRef ? mesh.m_MeshRef->GetId() : ULID{}, 1);
		dev_ui::ULIDWidget(
			"Material Instance",
			mesh.m_MaterialRef ? mesh.m_MaterialRef->GetId() : ULID{},
			2);
		return false;
	}
} // namespace brk::ecs