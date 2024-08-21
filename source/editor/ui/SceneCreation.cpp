#include "SceneCreation.hpp"

#ifdef BRK_EDITOR
#include <imgui.h>
#include <tinyfiledialogs.h>

namespace {
	constexpr const char* s_SceneFileExtension = "*.brkscn";
} // namespace

brk::editor::SceneCreationWindow brk::editor::SceneCreationWindow::s_Instance;

void brk::editor::SceneCreationWindow::Open() noexcept
{
	if (m_Show)
		return;
	m_Show = true;
	m_FilePath[0] = 0;
}

bool brk::editor::SceneCreationWindow::Show()
{
	if (!m_Show)
		return false;

	if (!ImGui::Begin("New Scene", &m_Show))
	{
		ImGui::End();
		return false;
	}

	ImGui::InputText("File Path", m_FilePath, sizeof(m_FilePath));

	if (ImGui::Button("Choose path"))
	{
		const char* path = tinyfd_saveFileDialog(
			"Select a location to save the file",
			nullptr,
			1,
			&s_SceneFileExtension,
			"");

		if (path)
			strcpy_s(m_FilePath, path);
	}

	const bool res = ImGui::Button("Create") && m_FilePath;

	ImGui::End();
	m_Show = !res;
	return res;
}

#endif // BRK_EDITOR