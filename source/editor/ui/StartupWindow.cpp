#include "StartupWindow.hpp"

#ifdef BRK_EDITOR

#include <tinyfiledialogs.h>
#include <array>
#include <editor/Editor.hpp>
#include <imgui.h>
#include <filesystem>

namespace {
	const char* s_BrkExt = "*.brk";
}

void brk::editor::StartupWindow()
{
	ImGui::Begin("Welcome", nullptr, ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoDocking);
	ImGui::Text("Welcome to the Breakout engine editor! To continue, either create a new "
				"project or load an existing one.");
	if (ImGui::Button("Load Project"))
	{
		const char* filePath =
			tinyfd_openFileDialog("Open Project", "", 1, &s_BrkExt, nullptr, 0);

		if (filePath)
		{
			Editor::GetInstance().LoadProjectDeferred(filePath);
		}
	}
	ImGui::End();
}

#endif