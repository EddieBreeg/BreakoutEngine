#include "SceneSelection.hpp"
#ifdef BRK_EDITOR
#include <managers/SceneManager.hpp>
#include <editor/Editor.hpp>

#include <imgui.h>

void brk::editor::SceneSelectionWindow(const TULIDMap<SceneDescription>& scenes)
{
	ImGui::Begin("Select a scene", nullptr);

	for (const auto& [id, descr] : scenes)
	{
		if (ImGui::Button(descr.GetName().c_str()))
		{
			Editor::GetInstance().LoadSceneDeferred(id);
			break;
		}
	}
	ImGui::End();
}

#endif