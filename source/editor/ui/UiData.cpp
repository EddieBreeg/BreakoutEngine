#include "UiData.hpp"
#include <ecs/ComponentFwd.hpp>
#include <managers/SceneManager.hpp>

void brk::editor::ui::UiData::Display(
	ecs::EntityWorld& world,
	ResourceManager& resourceManager,
	SceneManager& sceneManager,
	const ecs::ComponentRegistry& componentRegistry)
{
	MenuBar();

	if (m_ShowStartupWindow)
	{
		StartupWindow();
		return;
	}

	if (m_ShowSceneSelector)
	{
		SceneSelection(sceneManager.GetSceneDesriptions());
	}

	Outliner(sceneManager);
	Inspector(world, sceneManager, componentRegistry);
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

bool brk::editor::ui::UiData::ComponentDeletePopup::Display()
{
	if (!m_Show)
		return false;

	ImGui::OpenPopup(s_StrDeleteComponent);

	if (!ImGui::BeginPopup(s_StrDeleteComponent))
		return false;

	ImGui::Text("Delete %s?", m_Component->m_Info->m_Name.GetPtr());

	if (ImGui::Button("Delete"))
	{
		m_Show = false;
		ImGui::EndPopup();
		return true;
	}

	ImGui::SameLine();
	m_Show = !ImGui::Button("Cancel");

	ImGui::EndPopup();
	return false;
}
