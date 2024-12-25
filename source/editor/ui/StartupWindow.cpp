#include "UiData.hpp"
#if BRK_EDITOR

#include <editor/Editor.hpp>
#include <imgui.h>
#include <imgui/DevUiContext.hpp>
#include <SDL3/SDL_dialog.h>

namespace {
	constexpr SDL_DialogFileFilter s_ProjectFileFilter{ "Project File", "brk" };
}

void brk::editor::ui::UiData::StartupWindow()
{
	ImGui::Begin("Welcome", nullptr, ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoDocking);
	ImGui::Text("Welcome to the Breakout engine editor! To continue, either create a new "
				"project or load an existing one.");
	if (ImGui::Button("Load Project"))
	{
		SDL_ShowOpenFileDialog(
			OpenProjectCallback,
			this,
			dev_ui::Context::s_Instance.GetWindow(),
			&s_ProjectFileFilter,
			1,
			nullptr,
			false);
	}
	ImGui::End();
}

#endif