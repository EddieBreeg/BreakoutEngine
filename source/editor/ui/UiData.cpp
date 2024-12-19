#include "UiData.hpp"
#include <managers/SceneManager.hpp>

void brk::editor::ui::UiData::Display(
	ecs::EntityWorld& world,
	ResourceManager& resourceManager,
	SceneManager& sceneManager)
{
	MenuBar();

	if (m_ShowStartupWindow)
	{
		StartupWindow();
		return;
	}

	if (m_ShowSceneCreationWindow)
	{
		SceneCreation();
		return;
	}

	if (!m_SceneId)
	{
		SceneSelection(sceneManager.GetSceneDesriptions());
		return;
	}

	Outliner(sceneManager);
	Inspector(world, sceneManager);
	ResourceExplorer(resourceManager);
	ResourceEditor();
	if (m_ShowResourceCreationWindow)
		ResourceCreationWindow(resourceManager);
	m_ModalPopup.Display();
}

void brk::editor::ui::UiData::ModalPopup::Open(const char* title, std::string content)
{
	m_Title = title;
	m_Content = std::move(content);
	m_Show = true;
}

void brk::editor::ui::UiData::ModalPopup::Display()
{
	if (!m_Show)
		return;
	ImGui::OpenPopup(m_Title);
	if (!ImGui::BeginPopupModal(m_Title, &m_Show))
		return;
	ImGui::TextUnformatted(m_Content.c_str(), m_Content.c_str() + m_Content.length());
	ImGui::EndPopup();
}
