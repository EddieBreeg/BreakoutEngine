#include "StartupWindow.hpp"
#include <tinyfiledialogs.h>
#include <array>
#include <imgui.h>
#include <filesystem>

std::string_view brk::editor::StartupWindow()
{
	std::string_view filePath;

	ImGui::Begin("Welcome", nullptr, ImGuiWindowFlags_Modal);
	ImGui::Text("Welcome to the Breakout engine editor! To continue, either create a new "
				"project or load an existing one.");
	if (ImGui::Button("Load Project"))
	{
		static constexpr std::array patterns = { "*.brkproj", "*.brk" };

		const std::string defaultPath = std::filesystem::current_path().u8string();

		filePath = tinyfd_openFileDialog(
			"Open Project",
			defaultPath.c_str(),
			(int)patterns.size(),
			patterns.data(),
			nullptr,
			0);
	}
	ImGui::End();
	return filePath;
}