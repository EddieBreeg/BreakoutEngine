#include "Outliner.hpp"

#ifdef BRK_EDITOR
#include <managers/SceneManager.hpp>

#include <imgui.h>

brk::editor::Outliner brk::editor::Outliner::s_Instance;

bool brk::editor::Outliner::Display()
{
	if (!ImGui::Begin("Outliner"))
	{
		ImGui::End();
		return false;
	}

	if (!ImGui::BeginListBox("Objects"))
		return false;
	bool result = false;

	for (const auto& [id, obj] : SceneManager::GetInstance().GetGameObjects())
	{
		char idStr[27];
		idStr[26] = '\0';
		id.ToChars(idStr);
		const bool isSelected = obj.m_Entity == m_SelectedEntity;

		if (ImGui::Selectable(idStr, isSelected))
		{
			result |= m_SelectedEntity != obj.m_Entity;
			m_SelectedEntity = obj.m_Entity;
		}
		if (isSelected)
			ImGui::SetItemDefaultFocus();
	}
	ImGui::EndListBox();

	ImGui::End();
	return result;
}

#endif
