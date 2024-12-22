#include "UiData.hpp"
#ifdef BRK_EDITOR
#include <editor/Editor.hpp>
#include <tinyfiledialogs.h>
#include <array>
#include <imgui.h>

namespace {
	const char* s_BrkExt = "*.brk";
} // namespace

void brk::editor::ui::UiData::MenuBar()
{
	if (!ImGui::BeginMainMenuBar())
		return;

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Open Project", "Ctrl+Shift+O"))
		{
			const char* filePath =
				tinyfd_openFileDialog("Open Project", "", 1, &s_BrkExt, nullptr, 0);
			if (filePath)
			{
				m_FilePath = filePath;
				m_ProjectLoadRequested = true;
			}
		}

		if (ImGui::MenuItem("New Scene", "Ctrl+N"))
		{
			OpenSceneCreationWindow();
		}

		m_SceneSaveRequested = ImGui::MenuItem("Save Scene", "Ctrl+S", false, m_SceneId);
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}
#endif