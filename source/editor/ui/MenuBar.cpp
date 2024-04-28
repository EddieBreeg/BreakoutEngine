#include "Menubar.hpp"
#ifdef BRK_EDITOR
#include <editor/Editor.hpp>
#include <tinyfiledialogs.h>
#include <array>
#include <imgui.h>

void brk::editor::MenuBar()
{
	if (!ImGui::BeginMainMenuBar())
		return;

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Open Project", "Ctrl+Shift+O"))
		{
			static constexpr std::array patterns = { "*.brkproj", "*.brk" };
			const std::string_view filePath = tinyfd_openFileDialog(
				"Open Project",
				nullptr,
				(int)patterns.size(),
				patterns.data(),
				nullptr,
				0);
			Editor::GetInstance().LoadProjectDeferred(filePath);
		}
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}
#endif