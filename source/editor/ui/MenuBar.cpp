#include "UiData.hpp"
#if BRK_EDITOR
#include <editor/Editor.hpp>
#include <array>
#include <imgui.h>
#include <imgui/DebugOverlay.hpp>
#include <imgui/DevUiContext.hpp>
#include <SDL3/SDL_dialog.h>

namespace {
	constexpr SDL_DialogFileFilter s_ProjectFileFilter{ "Project File", "brk" };
	constexpr SDL_DialogFileFilter s_SceneFileFilter{ "Scene File", "brkscn" };

	void SceneSaveCallback(void* ptr, const char* const* filelist, int)
	{
		using brk::editor::ui::UiData;

		if (!filelist || !filelist[0])
			return;

		UiData* data = static_cast<UiData*>(ptr);
		data->m_FilePath = filelist[0];
		data->m_SceneSaveRequested = true;
	}

	void FileMenu(brk::editor::ui::UiData& data)
	{
		using namespace brk;

		if (!ImGui::BeginMenu("File"))
			return;

		SDL_Window* const window = dev_ui::Context::s_Instance.GetWindow();

		if (ImGui::MenuItem("Open Project", "Ctrl+Shift+O"))
		{
			SDL_ShowOpenFileDialog(
				editor::ui::OpenProjectCallback,
				&data,
				window,
				&s_ProjectFileFilter,
				1,
				nullptr,
				false);
		}

		data.m_ShowSceneSelector = ImGui::MenuItem("Open Scene", "Ctrl+O");
		data.m_NewSceneRequested = ImGui::MenuItem("New Scene", "Ctrl+N");

		if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
		{
			if (data.m_SceneId)
			{
				data.m_SceneSaveRequested = true;
			}
			else
			{
				SDL_ShowSaveFileDialog(
					SceneSaveCallback,
					&data,
					window,
					&s_SceneFileFilter,
					1,
					nullptr);
			}
		}

		ImGui::EndMenu();
	}

	void ViewMenu(brk::editor::ui::UiData&)
	{
		if (!ImGui::BeginMenu("View"))
			return;

		using brk::dbg::Overlay;

		ImGui::MenuItem("Debug Overlay", nullptr, &Overlay::s_Instance.m_Enabled);

		ImGui::EndMenu();
	}
} // namespace

void brk::editor::ui::UiData::MenuBar()
{
	if (!ImGui::BeginMainMenuBar())
		return;

	FileMenu(*this);
	ViewMenu(*this);

	ImGui::EndMainMenuBar();
}
#endif