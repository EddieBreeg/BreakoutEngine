#include "UiData.hpp"
#if BRK_EDITOR
#include <managers/SceneManager.hpp>
#include <editor/Editor.hpp>

#include <imgui.h>

void brk::editor::ui::UiData::SceneSelection(const TULIDMap<SceneDescription>& scenes)
{
	if (!ImGui::Begin("Select a scene", &m_ShowSceneSelector))
		return;

	for (const auto& [id, descr] : scenes)
	{
		if (ImGui::Button(descr.GetName().c_str()))
		{
			m_SceneLoadRequested = true;
			m_SceneId = id;
			break;
		}
	}
	ImGui::End();
}

#endif