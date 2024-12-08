#include "ComponentWidgets.hpp"
#include <imgui.h>
#include <imgui_macros.hpp>
#include <math/Constants.hpp>
#include <systems/TransformComponent.hpp>
#include <systems/VisualComponents.hpp>

bool brk::editor::TransformWidget(brk::TransformComponent& transform)
{
	bool res = IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Translate", &transform.m_Translate.x);
	res |= IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Rotation", &transform.m_EulerAngles.x);
	transform.m_Rotation = { (math::Pi / 180.f) * transform.m_EulerAngles };
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
