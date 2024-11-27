#include "Editor.hpp"

#ifdef BRK_EDITOR
#include "ui/Inspector.hpp"
#include "ui/Menubar.hpp"
#include "ui/Outliner.hpp"
#include "ui/SceneCreation.hpp"
#include "ui/SceneSelection.hpp"
#include "ui/StartupWindow.hpp"

#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <core/ULIDFormatter.hpp>

#include <managers/ECSManager.hpp>
#include <managers/SceneManager.hpp>

#include <cerrno>
#include <filesystem>
#include <fstream>
#include <system_error>

brk::editor::Editor::Editor(
	ecs::Manager& ecsManager,
	SceneManager& sceneManager,
	int argc,
	const char** argv)
	: m_ECSManager{ ecsManager }
	, m_SceneManager{ sceneManager }
{
	if (argc < 2)
		return;
	LoadProjectDeferred(argv[1]);
}

void brk::editor::Editor::LoadProjectDeferred(const StringView filePath) noexcept
{
	m_LoadState = LoadState::Project;
	m_ProjectFilePath = std::string{ filePath.GetPtr(), filePath.GetLen() };
}

void brk::editor::Editor::LoadSceneDeferred(const ULID sceneId) noexcept
{
	m_LoadState = LoadState::Scene;
	m_CurrentScene = sceneId;
}

void brk::editor::Editor::CreateNewScene(const char* path)
{
	const std::filesystem::path scenePath = std::filesystem::relative(path);
	const std::string name = scenePath.filename().stem().u8string();

	const SceneDescription& sceneDesc =
		SceneManager::GetInstance().CreateNewScene(std::move(name), scenePath.u8string());

	std::ofstream outFile{ scenePath };
	BRK_ASSERT(outFile.is_open(), "Couldn't create {}", sceneDesc.GetPath());

	nlohmann::json json{
		{ "name", sceneDesc.GetName() },
		{ "id", sceneDesc.GetId() },
	};

	outFile << json.dump(4);
}

void brk::editor::Editor::SaveProjectFile()
{
	nlohmann::json json;
	JsonLoader<Project>::Save(*m_Project, json);
	nlohmann::json& scenesArr = json["scenes"] = nlohmann::json::array();

	nlohmann::json sceneJson;
	for (const auto& [id, desc] : SceneManager::GetInstance().GetSceneDesriptions())
	{
		JsonLoader<SceneDescription>::Save(desc, sceneJson);
		scenesArr.emplace_back(std::move(sceneJson));
	}
	std::ofstream projectFile{ m_ProjectFilePath };
	BRK_ASSERT(
		projectFile.is_open(),
		"Couldn't save project file: {}",
		std::strerror(errno));
	projectFile << json.dump(4);
}

void brk::editor::Editor::Update()
{
	switch (m_LoadState)
	{
	case LoadState::Project:
		LoadProject();
		Outliner::s_Instance.Open();
		break;
	case LoadState::Scene: LoadScene(); break;
	default: break;
	}

	if (m_NewSceneRequested)
	{
		m_NewSceneRequested = false;
		CreateNewScene(SceneCreationWindow::s_Instance.GetScenePath());
		SaveProjectFile();
	}
}

void brk::editor::Editor::LoadProject()
{
	m_LoadState = LoadState::None;
	std::ifstream projectFile{ m_ProjectFilePath };
	if (!projectFile.is_open())
	{
		BRK_LOG_WARNING(
			"Couldn't open project file '{}': {}",
			m_ProjectFilePath,
			std::strerror(errno));
		m_ProjectFilePath = {};
		return;
	}
	BRK_LOG_TRACE("Loading project '{}'", m_ProjectFilePath);
	Project proj;
	proj.m_ProjectDir = std::filesystem::path(m_ProjectFilePath).parent_path();
	if (proj.m_ProjectDir.empty())
	{
		proj.m_ProjectDir = ".";
	}
	else
	{
		std::filesystem::current_path(proj.m_ProjectDir);
	}

	nlohmann::json desc;
	try
	{
		desc = nlohmann::json::parse(projectFile);
	}
	catch (const nlohmann::json::parse_error& err)
	{
		BRK_LOG_CRITICAL("Failed to parse {}: {}", m_ProjectFilePath, err.what());
		return;
	}

	JsonLoader<Project>::Load(proj, desc);
	m_Project = std::move(proj);

	const auto it = desc.find("scenes");
	if (it != desc.end())
	{
		SceneManager::GetInstance().LoadSceneDescriptions(*it);
	}

	BRK_LOG_TRACE("Finished loading project");
}

void brk::editor::Editor::LoadScene()
{
	BRK_LOG_TRACE("Loading scene {}", m_CurrentScene);
	SceneManager::GetInstance().LoadScene(m_CurrentScene);
	m_LoadState = LoadState::None;
}

void brk::editor::Editor::ShowUI()
{
	MenuBar();

	if (!m_Project.has_value())
	{
		StartupWindow();
		return;
	}

	const TULIDMap<SceneDescription>& scenes = m_SceneManager.GetSceneDesriptions();
	if (scenes.empty())
		SceneCreationWindow::s_Instance.Open();

	if (SceneCreationWindow::s_Instance.IsOpen())
	{
		m_NewSceneRequested = SceneCreationWindow::s_Instance.Show();
		return;
	}

	if (!m_CurrentScene)
	{
		SceneSelectionWindow(scenes);
		return;
	}

	if (Outliner::s_Instance.Display(m_SceneManager))
	{
		Inspector::s_Instance.Open();
		Inspector::s_Instance.m_SelectedObjectId =
			Outliner::s_Instance.GetSelectedObjectId();
	}

	Inspector::s_Instance.Display(m_ECSManager.GetWorld(), m_SceneManager);
}

#endif