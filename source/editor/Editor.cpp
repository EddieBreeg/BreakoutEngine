#include "Editor.hpp"

#ifdef BRK_EDITOR
#include "ui/Menubar.hpp"
#include "ui/StartupWindow.hpp"

#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <core/SceneManager.hpp>

#include <fstream>
#include <system_error>

brk::editor::Editor::Editor(int argc, const char** argv)
{
	if (argc < 2)
		return;
	LoadProjectDeferred(argv[1]);
}

void brk::editor::Editor::LoadProjectDeferred(const std::string_view filePath) noexcept
{
	m_ProjectFilePath = filePath;
}

void brk::editor::Editor::Update()
{
	if (m_ProjectFilePath.data())
	{
		LoadProject();
	}
}

void brk::editor::Editor::LoadProject()
{
	std::ifstream projectFile{ m_ProjectFilePath.data() };
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
	m_ProjectFilePath = {};

	const nlohmann::json desc = nlohmann::json::parse(projectFile);
	Project proj;
	JsonLoader<Project>::Load(proj, desc);
	m_Project = std::move(proj);

	const auto it = desc.find("scenes");
	if (it != desc.end())
	{
		SceneManager::GetInstance().LoadSceneDescriptions(*it);
	}

	LogManager::GetInstance().Log(LogManager::Trace, "Finished loading project");
}

void brk::editor::Editor::ShowUI()
{
	MenuBar();

	if (!m_Project.has_value())
	{
		StartupWindow();
	}
}

#endif