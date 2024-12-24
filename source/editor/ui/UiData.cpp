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
	Inspector(world, componentRegistry);
	ResourceExplorer(resourceManager);
	ResourceEditor();
	if (m_ShowResourceCreationWindow)
		ResourceCreationWindow(resourceManager);
	m_ModalPopup.Display();
}

void brk::editor::ui::UiData::ModalPopup::Open(
	const char* title,
	std::string content,
	bool* confirmed)
{
	m_Title = title;
	m_Confirmed = confirmed;
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
	if (m_Confirmed)
	{
		bool confirmed = ImGui::Button("Confirm");
		ImGui::SameLine();
		m_Show = !(confirmed || ImGui::Button("Cancel"));
		*m_Confirmed = confirmed;
	}

	ImGui::EndPopup();
}

void brk::editor::ui::OpenProjectCallback(
	void* ptr,
	const char* const* filelist,
	int)
{
	using brk::editor::ui::UiData;

	if (!filelist || !filelist[0])
		return;

	UiData* data = static_cast<UiData*>(ptr);
	data->m_FilePath = filelist[0];
	data->m_ProjectLoadRequested = true;
}
