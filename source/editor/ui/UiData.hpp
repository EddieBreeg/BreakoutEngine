#pragma once

#include <PCH.hpp>

#ifdef BRK_EDITOR
#include <core/ResourceFwd.hpp>
#include <core/ULID.hpp>
#include <ecs/GameObject.hpp>
#include <ecs/WorldFwd.hpp>
#include <filesystem>
#include <imgui.h>

namespace {
	static constexpr const char* s_StrDeleteComponent = "Delete Component?";
}

namespace brk {
	class Resource;
	class ResourceManager;
	class ResourceTypeInfo;
	class SceneDescription;
	class SceneManager;
} // namespace brk

namespace brk::ecs {
	class ComponentRegistry;

	struct ComponentInfo;
} // namespace brk::ecs

namespace brk::editor::ui {
	static constexpr const char* s_StrInspector = "Inspector";
	static constexpr const char* s_StrOutliner = "Outliner";
	static constexpr const char* s_StrResourceExplorer = "Resource Explorer";

	struct BRK_EDITOR_API UiData
	{
		void Display(
			ecs::EntityWorld& world,
			ResourceManager& resourceManager,
			SceneManager& sceneManager,
			const ecs::ComponentRegistry& componentRegistry);

		void MenuBar();
		void StartupWindow();
		void SceneSelection(const TULIDMap<SceneDescription>& sceneDescriptions);
		bool SceneCreation();

		bool Outliner(SceneManager& sceneManager);
		bool Inspector(
			ecs::EntityWorld& entityWorld,
			SceneManager& sceneManager,
			const ecs::ComponentRegistry& componentRegistry);

		void ResourceExplorer(const ResourceManager& resourceManager);
		Resource* ResourceCreationWindow(ResourceManager& resourceManager);
		void ResourceEditor();

		void OpenSceneCreationWindow();

		bool m_ShowStartupWindow = false;
		bool m_ShowSceneSelector = false;
		bool m_ShowOutliner = false;
		bool m_ShowResourceCreationWindow = false;

		bool m_ProjectLoadRequested = false;
		bool m_SceneLoadRequested = false;
		bool m_NewSceneRequested = false;
		bool m_AddResourceRequested = false;
		std::atomic_bool m_SceneSaveRequested = false;

		bool m_LayoutResetRequested = false;

		std::filesystem::path m_FilePath;

		struct ComponentTypeSelector
		{
			const ecs::ComponentInfo* m_Selection = nullptr;
			ImGuiTextFilter m_NameFilter;
			bool m_Show = false;
		};

		struct
		{
			ecs::GameObject* m_SelectedObject = nullptr;
			ComponentTypeSelector m_TypeSelector;
			uint32 m_DeletedComponent = ~0u;
			bool m_AddComponentRequested = false;
			bool m_CreateObjectRequested = false;
			bool m_DeleteObjectRequested = false;
			bool m_DeleteComponentRequested = false;
		} m_InspectorData;

		struct
		{
			ImGuiTextFilter m_ResourceNameFilter;
			const ResourceTypeInfo* m_TypeFilter = nullptr;
		} m_ResourceExplorerData;

		struct
		{
			ResourceTypeInfo* m_Info = {};
			Resource* m_Resource = nullptr;
			bool m_ReloadRequested = false;
			bool m_SaveRequested = false;
			bool m_DeletionRequested = false;
		} m_ResourceEditorData;

		struct ModalPopup
		{
			bool m_Show = false;
			bool* m_Confirmed = nullptr;
			const char* m_Title = nullptr;
			std::string m_Content;
			void Open(const char* title, std::string content, bool* confirmed = nullptr);
			void Display();
		} m_ModalPopup;

		struct
		{
			ResourceTypeInfo* m_Info = {};
			std::unique_ptr<Resource, ResourceDeleter> m_Resource;
		} m_ResourceCreationData;

		ULID m_SceneId;
	};

} // namespace brk::editor::ui
#endif