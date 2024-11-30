#include "ComponentWidgets.hpp"
#include <imgui.h>
#include <imgui_macros.hpp>
#include <systems/TransformComponent.hpp>
#include <systems/VisualComponents.hpp>

bool brk::editor::TransformWidget(brk::TransformComponent& transform)
{
	bool res = IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Translate", &transform.m_Translate.x);
	float3 euler = glm::eulerAngles(transform.m_Rotation);
	res |= IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Rotation", &euler.x);
	transform.m_Rotation = math::Quaternion{ euler };
	res |= IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Scale", &transform.m_Scale.x);
	return res;
}

bool brk::editor::MeshComponentWidget(MeshComponent& mesh)
{
	char idStr[27] = {};
	mesh.m_MeshRef->GetId().ToChars(idStr);
	ImGui::Text("Mesh: %s", idStr);
	mesh.m_MaterialRef->GetId().ToChars(idStr);
	ImGui::Text("Materian Instance: %s", idStr);
	return false;
}
