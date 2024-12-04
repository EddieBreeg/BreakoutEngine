#include "UiData.hpp"
#ifdef BRK_EDITOR
#include <imgui.h>
#include <tinyfiledialogs.h>

namespace {
	constexpr const char* s_SceneFileExtension = "*.brkscn";
} // namespace

bool brk::editor::ui::UiData::SceneCreation()
{
	if (!m_ShowSceneCreationWindow)
		return false;

	if (!ImGui::Begin("New Scene", &m_ShowSceneCreationWindow))
	{
		ImGui::End();
		return false;
	}

	ImGui::Text("Selected path: %s", m_FilePath);

	if (ImGui::Button("Choose file"))
	{
		char* path = tinyfd_saveFileDialog(
			"Select a location to save the file",
			nullptr,
			1,
			&s_SceneFileExtension,
			"");

		if (path)
		{
			m_FilePath = path;
		}
	}

	ImGui::BeginDisabled(!m_FilePath);
	m_NewSceneRequested = ImGui::Button("Create") && m_FilePath;
	ImGui::EndDisabled();

	ImGui::End();
	m_ShowSceneCreationWindow = !m_NewSceneRequested;
	return m_NewSceneRequested;
}

void brk::editor::ui::UiData::OpenSceneCreationWindow()
{
	if (m_ShowSceneCreationWindow)
		return;
	m_FilePath = nullptr;
	m_ShowSceneCreationWindow = true;
}

#endif // BRK_EDITOR