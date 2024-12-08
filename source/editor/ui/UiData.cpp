#include "UiData.hpp"
#include <managers/SceneManager.hpp>

void brk::editor::ui::UiData::Display(
	entt::registry& world,
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
	ResourceExplorer();
	if (m_ShowResourceCreationWindow)
		ResourceCreationWindow(resourceManager);
}