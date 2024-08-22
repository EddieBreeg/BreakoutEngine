#include "ComponentWidgets.hpp"
#include <imgui.h>
#include <imgui_macros.hpp>
#include <systems/TransformComponent.hpp>

bool brk::editor::TransformWidget(brk::TransformComponent& transform)
{
	return IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Translate", &transform.m_Translate.x);
}