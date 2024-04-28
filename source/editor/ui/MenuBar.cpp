#include "Menubar.hpp"
#include <imgui.h>

void brk::MenuBar()
{
	if (!ImGui::BeginMainMenuBar())
		return;

	if (ImGui::BeginMenu("File"))
	{
		ImGui::MenuItem("Open Project", "Ctrl+Shift+O");
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}