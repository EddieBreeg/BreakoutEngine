#include "Editor.hpp"

#ifdef BRK_EDITOR
#include "ui/UiData.hpp"

#include <app/App.hpp>

#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <core/ResourceLoader.hpp>
#include <core/ULIDFormatter.hpp>

#include <ecs/ComponentRegistry.hpp>

#include <imgui.h>
#include <imgui_internal.h>

#include <managers/ECSManager.hpp>
#include <managers/ResourceManager.hpp>
#include <managers/SceneManager.hpp>

#include <cerrno>
#include <filesystem>
#include <fstream>
#include <system_error>

namespace {
	void InitUiLayout()
	{
		using namespace brk::editor;

		const ImGuiID dockspaceId = 1;
		ImGuiID node1 = dockspaceId, node2 = 0;

		ImGui::DockBuilderSplitNode(node1, ImGuiDir_Left, .25f, &node1, &node2);
		ImGui::DockBuilderDockWindow(ui::s_StrOutliner, node1);

		ImGui::DockBuilderSplitNode(node2, ImGuiDir_Right, .33f, &node1, nullptr);
		ImGui::DockBuilderSplitNode(node1, ImGuiDir_Up, .67f, &node1, &node2);

		ImGui::DockBuilderDockWindow(ui::s_StrInspector, node1);
		ImGui::DockBuilderDockWindow(ui::s_StrResourceExplorer, node2);

		ImGui::DockBuilderFinish(dockspaceId);
	}

	brk::ULID CreateScene(brk::SceneManager& manager, std::filesystem::path path)
	{
		using namespace brk;
		std::string pathStr = std::filesystem::relative(path).u8string();

		const uint32 nameEnd = (uint32)Min(pathStr.rfind('.'), pathStr.length());
		const char* nameStart = pathStr.c_str() + nameEnd;
		{
			const char* end = pathStr.c_str();
			while (nameStart != end)
			{
				if (nameStart[-1] == '/' || nameStart[-1] == '\\')
					break;
				--nameStart;
			}
		}
		std::string name{ nameStart, pathStr.c_str() + nameEnd };
		return manager.CreateNewScene(std::move(name), std::move(pathStr)).GetId();
	}
} // namespace

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

	m_UiData->m_LayoutResetRequested =
		!std::filesystem::is_regular_file(ImGui::GetIO().IniFilename);

	if (argc < 2)
		return;
	m_UiData->m_ProjectLoadRequested = true;
	m_UiData->m_FilePath = argv[1];
}

void brk::editor::Editor::SaveProjectFile()
{
	nlohmann::json json;
	JsonLoader<Project>::Save(*m_Project, json);
	nlohmann::json& scenesArr = json["scenes"] = nlohmann::json::array();

	nlohmann::json sceneJson;
	for (const auto& [id, desc] : m_SceneManager.GetSceneDesriptions())
	{
		JsonLoader<SceneDescription>::Save(desc, sceneJson);
		scenesArr.emplace_back(std::move(sceneJson));
	}

	nlohmann::json& resourcesArr = json["resources"] = nlohmann::json::array();
	nlohmann::json resJson;
	for (const auto& [id, resource] : m_ResourceManager.GetResources())
	{
		if (resource->GetSavingDisabled())
			continue;
		const ResourceTypeInfo& info = resource->GetTypeInfo();
		JsonLoader<Resource>::Save(*resource, resJson);
		if (info.m_Save)
			info.m_Save(*resource, resJson);
		resourcesArr.emplace_back(std::move(resJson));
	}

	std::ofstream projectFile{ m_ProjectFilePath };
	BRK_ASSERT(projectFile.is_open(), "Couldn't save project file: {}", StrError(errno));
	projectFile << json.dump(4);
}

void brk::editor::Editor::Update()
{
	ecs::EntityWorld& world = m_ECSManager.GetWorld();

	if (m_UiData->m_ProjectLoadRequested)
	{
		m_UiData->m_ProjectLoadRequested = false;
		LoadProject(std::move(m_UiData->m_FilePath));
		m_UiData->m_ShowSceneSelector = true;
		return;
	}
	if (m_UiData->m_SceneLoadRequested)
	{
		m_UiData->m_SceneLoadRequested = false;
		m_UiData->m_ShowSceneSelector = false;
		LoadScene(m_UiData->m_SceneId);
		return;
	}

	if (m_UiData->m_NewSceneRequested)
	{
		m_UiData->m_NewSceneRequested = false;
		m_UiData->m_InspectorData.m_SelectedObject = nullptr;
		m_UiData->m_SceneId = {};
		world.clear();
		m_SceneManager.ClearCurrentScene();
	}

	if (m_UiData->m_SceneSaveRequested)
	{
		if (!m_UiData->m_SceneId)
		{
			m_UiData->m_SceneId =
				CreateScene(m_SceneManager, std::move(m_UiData->m_FilePath));
			SaveProjectFile();
		}
		m_UiData->m_SceneSaveRequested = false;
		m_SceneManager.SaveCurrentSceneToFile(world);
	}

	if (m_UiData->m_AddResourceRequested)
	{
		m_ResourceManager.AddResource(
			m_UiData->m_ResourceCreationData.m_Resource.release());
		m_UiData->m_ResourceCreationData = {};
		m_UiData->m_AddResourceRequested = false;
		SaveProjectFile();
	}

	if (m_UiData->m_ResourceEditorData.m_ReloadRequested)
	{
		ResourceLoader::GetInstance().AddJob(
			m_UiData->m_ResourceEditorData.m_Resource,
			ResourceLoader::EJobType::Reload);
		m_UiData->m_ResourceEditorData.m_ReloadRequested = false;
	}

	if (m_UiData->m_ResourceEditorData.m_SaveRequested)
	{
		m_UiData->m_ResourceEditorData.m_SaveRequested = false;
		SaveProjectFile();
	}

	if (m_UiData->m_ResourceEditorData.m_DeletionRequested)
	{
		m_UiData->m_ResourceEditorData.m_DeletionRequested = false;
		m_ResourceManager.DeleteResource(m_UiData->m_ResourceEditorData.m_Resource);
		m_UiData->m_ResourceEditorData.m_Resource = nullptr;
		SaveProjectFile();
	}

	if (m_UiData->m_InspectorData.m_CreateObjectRequested)
	{
		ecs::GameObject& object = m_SceneManager.CreateObject(world);
		m_UiData->m_InspectorData.m_SelectedObject = &object;
		m_UiData->m_InspectorData.m_CreateObjectRequested = false;
	}

	if (m_UiData->m_InspectorData.m_DeleteObjectRequested)
	{
		m_UiData->m_InspectorData.m_DeleteObjectRequested = false;
		world.destroy(m_UiData->m_InspectorData.m_SelectedObject->m_Entity);
		m_SceneManager.DeleteObject(m_UiData->m_InspectorData.m_SelectedObject->m_Id);
		m_UiData->m_InspectorData.m_SelectedObject = nullptr;
	}

	if (m_UiData->m_InspectorData.m_AddComponentRequested)
	{
		m_UiData->m_InspectorData.m_AddComponentRequested = false;
		ecs::GameObject* object = m_UiData->m_InspectorData.m_SelectedObject;
		const ecs::ComponentInfo* info =
			m_UiData->m_InspectorData.m_TypeSelector.m_Selection;
		const void* component = info->m_Add(world, object->m_Entity);
		void* widget = info->m_WidgetInfo.m_Create(component);

		object->m_Components.emplace_back(*info, widget);
	}

	if (m_UiData->m_InspectorData.m_DeleteComponentRequested)
	{
		ecs::GameObject* object = m_UiData->m_InspectorData.m_SelectedObject;

		auto it =
			object->m_Components.begin() + m_UiData->m_InspectorData.m_DeletedComponent;
		it->m_Info->m_Remove(world, object->m_Entity);
		object->m_Components.erase(it);
		m_UiData->m_InspectorData.m_DeleteComponentRequested = false;
	}
}

void brk::editor::Editor::LoadProject(const std::filesystem::path filePath)
{
	m_ProjectFilePath = filePath.u8string();
	std::ifstream projectFile{ filePath };
	if (!projectFile.is_open())
	{
		BRK_LOG_WARNING(
			"Couldn't open project file '{}': {}",
			m_ProjectFilePath,
			StrError(errno));
		m_ProjectFilePath = {};
		return;
	}
	BRK_LOG_TRACE("Loading project '{}'", m_ProjectFilePath);
	Project proj;
	proj.m_ProjectDir = filePath.parent_path();
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
	if (m_UiData->m_LayoutResetRequested)
	{
		InitUiLayout();
		m_UiData->m_LayoutResetRequested = false;
	}

	const auto& componentRegistry = ecs::ComponentRegistry::GetInstance();

	if ((m_UiData->m_ShowStartupWindow = !m_Project.has_value()))
	{
		m_UiData->Display(
			m_ECSManager.GetWorld(),
			m_ResourceManager,
			m_SceneManager,
			componentRegistry);
		return;
	}

	m_UiData->Display(
		m_ECSManager.GetWorld(),
		m_ResourceManager,
		m_SceneManager,
		componentRegistry);
}

#endif