#include "ComponentWidgets.hpp"
#include <imgui.h>
#include <systems/TransformComponent.hpp>

bool brk::editor::TransformWidget(brk::TransformComponent& transform)
{
	return ImGui::DragFloat3("Translate", &transform.m_Translate.x);
}