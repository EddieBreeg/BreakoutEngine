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

	m_InspectorData.m_CreateObjectRequested = ImGui::Button("Create Object");

	if (!ImGui::BeginListBox("Objects"))
		goto OUTLINER_END;

	bool result = false;

	for (auto&& [id, obj] : sceneManager.GetGameObjects())
	{
		const bool isSelected = m_InspectorData.m_SelectedObject &&
								(obj.m_Id == m_InspectorData.m_SelectedObject->m_Id);

		if (ImGui::Selectable(obj.m_Name.c_str(), isSelected))
		{
			result |= !isSelected;
			m_InspectorData.m_SelectedObject = &obj;
		}
		if (isSelected)
			ImGui::SetItemDefaultFocus();
	}
	ImGui::EndListBox();

OUTLINER_END:

	ImGui::End();
	return result;
}
#endif
