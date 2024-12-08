#include "Editor.hpp"

#ifdef BRK_EDITOR
#include "ui/UiData.hpp"

#include <app/App.hpp>

#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <core/ULIDFormatter.hpp>

#include <imgui.h>

#include <managers/ECSManager.hpp>
#include <managers/ResourceManager.hpp>
#include <managers/SceneManager.hpp>

#include <cerrno>
#include <filesystem>
#include <fstream>
#include <system_error>

std::unique_ptr<brk::editor::Editor> brk::editor::Editor::s_Instance;

brk::editor::Editor::Editor(
	ImGuiContext& ctx,
	ecs::Manager& ecsManager,
	ResourceManager& resourceManager,
	SceneManager& sceneManager,
	int argc,
	const char** argv)
	: m_UiData{ new ui::UiData{} }
	, m_ECSManager{ ecsManager }
	, m_ResourceManager{ resourceManager }
	, m_SceneManager{ sceneManager }
{
	ImGui::SetCurrentContext(&ctx);
	if (argc < 2)
		return;
	m_UiData->m_ProjectLoadRequested = true;
	m_UiData->m_FilePath = argv[1];
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
	if (m_UiData->m_ProjectLoadRequested)
	{
		m_UiData->m_ProjectLoadRequested = false;
		LoadProject(m_UiData->m_FilePath);
		return;
	}
	if (m_UiData->m_SceneLoadRequested)
	{
		m_UiData->m_SceneLoadRequested = false;
		LoadScene(m_UiData->m_SceneId);
		return;
	}

	if (m_UiData->m_NewSceneRequested)
	{
		m_UiData->m_NewSceneRequested = false;
		CreateNewScene(m_UiData->m_FilePath);
		SaveProjectFile();
	}
	if (m_UiData->m_AddResourceRequested)
	{
		m_ResourceManager.AddResource(m_UiData->m_ResourceCreationData.m_Resource);
		m_UiData->m_ResourceCreationData = {};
		m_UiData->m_AddResourceRequested = false;
	}
}

void brk::editor::Editor::LoadProject(const char* filePath)
{
	m_ProjectFilePath = filePath;
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

	auto it = desc.find("scenes");
	if (it != desc.end())
	{
		SceneManager::GetInstance().LoadSceneDescriptions(*it);
	}
	it = desc.find("resources");
	if (it != desc.end())
	{
		ResourceManager::GetInstance().CreateResources(*it);
	}

	BRK_LOG_TRACE("Finished loading project");
}

void brk::editor::Editor::LoadScene(ULID id)
{
	BRK_LOG_TRACE("Loading scene {}", id);
	SceneManager::GetInstance().LoadScene(id);
}

void brk::editor::Editor::ShowUI()
{
	const TULIDMap<SceneDescription>& scenes = m_SceneManager.GetSceneDesriptions();

	if (m_UiData->m_ShowStartupWindow = !m_Project.has_value())
	{
		m_UiData->Display(m_ECSManager.GetWorld(), m_ResourceManager, m_SceneManager);
		return;
	}

	if (scenes.empty())
	{
		m_UiData->OpenSceneCreationWindow();
	}

	m_UiData->Display(m_ECSManager.GetWorld(), m_ResourceManager, m_SceneManager);
}

#endif