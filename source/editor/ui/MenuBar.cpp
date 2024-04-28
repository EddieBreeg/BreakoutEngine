#include "Menubar.hpp"
#include <imgui.h>

void brk::MenuBar()
{
	ImGui::BeginMenuBar();
	ImGui::BeginMenu("File");

	ImGui::MenuItem("Open Project", "Ctrl+Shift+O");

	ImGui::EndMenu();
	ImGui::EndMenuBar();
}