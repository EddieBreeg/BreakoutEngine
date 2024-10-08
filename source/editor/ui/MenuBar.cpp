#include "Menubar.hpp"
#ifdef BRK_EDITOR
#include <editor/Editor.hpp>
#include <tinyfiledialogs.h>
#include <array>
#include <imgui.h>

#include "SceneCreation.hpp"

namespace {
	const char* s_BrkExt = "*.brk";
} // namespace

void brk::editor::MenuBar()
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
				Editor::GetInstance().LoadProjectDeferred(filePath);
			}
		}

		if (ImGui::MenuItem("New Scene", "Ctrl+N"))
		{
			SceneCreationWindow::s_Instance.Open();
		}

		ImGui::MenuItem("Save", "Ctrl+S", nullptr, false);
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}
#endif