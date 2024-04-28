#include "Editor.hpp"

#ifdef BRK_EDITOR
#include "ui/Menubar.hpp"
#include "ui/StartupWindow.hpp"

#include <core/Loaders.hpp>
#include <core/LogManager.hpp>

#include <fstream>
#include <system_error>

brk::editor::Editor::Editor(int argc, const char** argv)
{
	if (argc < 2)
		return;
	{
		std::ifstream projectFile{ argv[1] };
		if (!projectFile.is_open())
			return;

		const nlohmann::json desc = nlohmann::json::parse(projectFile);
		m_Project = JsonLoader<Project>::Load(desc);
		LogManager::GetInstance().Log(
			LogManager::Trace,
			"Loading project '{}'",
			m_Project->GetName());
	}
}

void brk::editor::Editor::LoadProjectDeferred(const std::string_view filePath) noexcept
{
	m_ProjectFilePath = filePath;
}

void brk::editor::Editor::Update()
{
	if (m_ProjectFilePath.data())
	{
		OnProjectFilePathChanged();
	}
}

void brk::editor::Editor::OnProjectFilePathChanged()
{
	// std::ifstream projectFile{ "foobar" };
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
	m_ProjectFilePath = {};

	const nlohmann::json desc = nlohmann::json::parse(projectFile);
	m_Project = JsonLoader<Project>::Load(desc);
	LogManager::GetInstance().Log(
		LogManager::Trace,
		"Loading project '{}'",
		m_Project->GetName());
}

void brk::editor::Editor::ShowUI()
{
	MenuBar();

	if (!m_Project.has_value())
	{
		m_ProjectFilePath = StartupWindow();
	}
}

#endif