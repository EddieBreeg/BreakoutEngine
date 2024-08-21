#include "Inspector.hpp"

#ifdef BRK_EDITOR
#include <core/ULIDFormatter.hpp>
#include <imgui.h>
#include <debug/GameObjectInfoComponent.hpp>
#include <managers/SceneManager.hpp>
#include <entt/entity/registry.hpp>

brk::editor::Inspector brk::editor::Inspector::s_Instance;

bool brk::editor::Inspector::Display(
	entt::registry& entityWorld,
	const SceneManager& sceneManager)
{
	if (!m_Show)
		return false;

	ImGui::Begin("Inspector", &m_Show);

	if (!m_SelectedObjectId)
	{
		ImGui::Text("No object selected");
		ImGui::End();
		return false;
	}

	const ecs::GameObject* object = sceneManager.GetObject(m_SelectedObjectId);
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
		ImGui::Text("Name: %s", object->m_Name.c_str());
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