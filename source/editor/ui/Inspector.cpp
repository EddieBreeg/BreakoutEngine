#include "UiData.hpp"
#ifdef BRK_EDITOR
#include <core/ULIDFormatter.hpp>
#include <imgui.h>
#include <imgui/MiscWidgets.hpp>
#include <imgui_macros.hpp>
#include <imgui_stdlib.h>
#include <imgui/GameObjectInfoComponent.hpp>
#include <managers/SceneManager.hpp>
#include <entt/entity/registry.hpp>

bool brk::editor::ui::UiData::Inspector(
	entt::registry& entityWorld,
	SceneManager& sceneManager)
{
	ImGui::Begin("Inspector");

	if (!m_SelectedObjectId)
	{
		ImGui::Text("No object selected");
		ImGui::End();
		return false;
	}

	ecs::GameObject* object = sceneManager.GetObject(m_SelectedObjectId);
	DEBUG_CHECK(object)
	{
		BRK_LOG_WARNING(
			"Object {} is selected but does not exist in the scene manager",
			m_SelectedObjectId);
		ImGui::End();
		return false;
	}

	if (ImGui::TreeNode("Object Info"))
	{
		dev_ui::ULIDWidget("ULID", object->m_Id);
		IMGUI_LEFT_LABEL(ImGui::InputText, "Name", &object->m_Name);
		ImGui::TreePop();
	}

	bool result = false;
	for (auto&& [info, widget] : object->m_Components)
	{
		if (!ImGui::TreeNode(info->m_Name.GetPtr()))
			continue;

		result |= info->m_WidgetInfo.m_Display(widget, entityWorld, object->m_Entity);
		ImGui::TreePop();
	}
	ImGui::End();

	return result;
}
#endif