#include "UiData.hpp"
#ifdef BRK_EDITOR
#include <editor/Editor.hpp>
#include <array>
#include <imgui.h>
#include <imgui/DevUiContext.hpp>
#include <SDL3/SDL_dialog.h>

namespace {
	constexpr SDL_DialogFileFilter s_ProjectFileFilter{ "Project File", "brk" };
	constexpr SDL_DialogFileFilter s_SceneFileFilter{ "Scene File", "brkscn" };

	void SceneSaveCallback(void* ptr, const char* const* filelist, int filter)
	{
		using brk::editor::ui::UiData;

		if (!filelist || !filelist[0])
			return;

		UiData* data = static_cast<UiData*>(ptr);
		data->m_FilePath = filelist[0];
		data->m_SceneSaveRequested = true;
	}
} // namespace

void brk::editor::ui::UiData::MenuBar()
{
	if (!ImGui::BeginMainMenuBar())
		return;

	SDL_Window* const window = dev_ui::Context::s_Instance.GetWindow();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Open Project", "Ctrl+Shift+O"))
		{
			SDL_ShowOpenFileDialog(
				OpenProjectCallback,
				this,
				window,
				&s_ProjectFileFilter,
				1,
				nullptr,
				false);
		}

		m_ShowSceneSelector = ImGui::MenuItem("Open Scene", "Ctrl+O");
		m_NewSceneRequested = ImGui::MenuItem("New Scene", "Ctrl+N");

		if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
		{
			if (m_SceneId)
			{
				m_SceneSaveRequested = true;
			}
			else
			{
				SDL_ShowSaveFileDialog(
					SceneSaveCallback,
					this,
					window,
					&s_SceneFileFilter,
					1,
					nullptr);
			}
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}
#endif