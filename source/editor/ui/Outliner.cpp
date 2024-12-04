#include "UiData.hpp"
#ifdef BRK_EDITOR
#include <managers/SceneManager.hpp>

#include <imgui.h>

bool brk::editor::ui::UiData::Outliner(SceneManager& sceneManager)
{
	if (!ImGui::Begin("Outliner"))
	{
		ImGui::End();
		return false;
	}

	if (!ImGui::BeginListBox("Objects"))
		return false;
	bool result = false;

	for (const auto& [id, obj] : sceneManager.GetGameObjects())
	{
		const bool isSelected = obj.m_Id == m_SelectedObjectId;

		if (ImGui::Selectable(obj.m_Name.c_str(), isSelected))
		{
			result |= !isSelected;
			m_SelectedObjectId = obj.m_Id;
		}
		if (isSelected)
			ImGui::SetItemDefaultFocus();
	}
	ImGui::EndListBox();

	ImGui::End();
	return result;
}
#endif
