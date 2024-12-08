#include "UiData.hpp"
#ifdef BRK_EDITOR
#include <core/ULIDFormatter.hpp>
#include <imgui.h>
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
		char idStr[27];
		idStr[26] = 0;
		object->m_Id.ToChars(idStr);
		ImGui::Text("ULID: %s", idStr);
		IMGUI_LEFT_LABEL(ImGui::InputText, "Name", &object->m_Name);
		ImGui::TreePop();
	}

	bool result = false;
	for (const ecs::ComponentInfo* comp : object->m_Components)
	{
		DEBUG_CHECK(comp->m_UiWidget)
		{
			BRK_LOG_WARNING(
				"Cannot display component {} in the inspector: no ui widget available",
				comp->m_Name);
			continue;
		}
		if (!ImGui::TreeNode(comp->m_Name))
			continue;

		result |= comp->m_UiWidget(entityWorld, object->m_Entity);
		ImGui::TreePop();
	}
	ImGui::End();

	return result;
}
#endif