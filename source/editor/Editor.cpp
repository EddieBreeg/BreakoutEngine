#include "Editor.hpp"

#ifdef BRK_EDITOR
#include "ui/Menubar.hpp"
#include "ui/SceneSelection.hpp"
#include "ui/StartupWindow.hpp"

#include <core/Loaders.hpp>
#include <core/LogManager.hpp>

#include <ecs/SceneManager.hpp>

#include <fstream>
#include <system_error>

brk::editor::Editor::Editor(int argc, const char** argv)
{
	if (argc < 2)
		return;
	LoadProjectDeferred(argv[1]);
}

void brk::editor::Editor::LoadProjectDeferred(const StringView filePath) noexcept
{
	m_LoadState = LoadState::Project;
	m_ProjectFilePath = filePath;
}

void brk::editor::Editor::LoadSceneDeferred(const ULID sceneId) noexcept
{
	m_LoadState = LoadState::Scene;
	m_CurrentScene = sceneId;
}

void brk::editor::Editor::Update()
{
	switch (m_LoadState)
	{
	case LoadState::Project: LoadProject(); break;
	case LoadState::Scene: LoadScene(); break;
	default: break;
	}
}

void brk::editor::Editor::LoadProject()
{
	m_LoadState = LoadState::None;
	std::ifstream projectFile{ m_ProjectFilePath.GetPtr() };
	if (!projectFile.is_open())
	{
		LogManager::GetInstance().Log(
			LogManager::Warning,
			"Couldn't open project file '{}': {}",
			m_ProjectFilePath,
			std::error_code{ errno, std::system_category() }.message());
		m_ProjectFilePath = {};
		return;
	}
	LogManager::GetInstance().Log(
		LogManager::Trace,
		"Loading project '{}'",
		m_ProjectFilePath);

	const nlohmann::json desc = nlohmann::json::parse(projectFile);
	Project proj;
	JsonLoader<Project>::Load(proj, desc);
	m_Project = std::move(proj);

	const auto it = desc.find("scenes");
	if (it != desc.end())
	{
		ecs::SceneManager::GetInstance().LoadSceneDescriptions(*it);
	}

	LogManager::GetInstance().Log(LogManager::Trace, "Finished loading project");
}

void brk::editor::Editor::LoadScene()
{
	ecs::SceneManager::GetInstance().LoadScene(m_CurrentScene);
	m_LoadState = LoadState::None;
}

void brk::editor::Editor::ShowUI()
{
	MenuBar();

	if (!m_Project.has_value())
	{
		StartupWindow();
	}
	else if (!m_CurrentScene)
	{
		SceneSelectionWindow();
	}
}

#endif