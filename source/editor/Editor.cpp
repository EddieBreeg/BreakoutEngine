#include "Editor.hpp"

#ifdef BRK_DEV
#include "ui/Menubar.hpp"
#include <core/LogManager.hpp>
#include <fstream>

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

void brk::editor::Editor::ShowUI()
{
	MenuBar();
}

#endif