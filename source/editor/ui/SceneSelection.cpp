#include "SceneSelection.hpp"
#ifdef BRK_EDITOR
#include <core/SceneManager.hpp>
#include <editor/Editor.hpp>

#include <imgui.h>

void brk::editor::SceneSelectionWindow()
{
	ImGui::Begin(
		"Select a scene",
		nullptr,
		ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoDocking);

	for (const auto& [id, descr] : SceneManager::GetInstance().GetObjects())
	{
		if (ImGui::Button(descr.GetPath().c_str()))
		{
			Editor::GetInstance().LoadSceneDeferred(id);
			break;
		}
	}
	ImGui::End();
}

#endif