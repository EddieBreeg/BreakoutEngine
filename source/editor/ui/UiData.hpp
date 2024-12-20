#pragma once

#include <PCH.hpp>

#ifdef BRK_EDITOR
#include <core/ResourceFwd.hpp>
#include <core/ULID.hpp>
#include <ecs/WorldFwd.hpp>
#include <imgui.h>

namespace brk {
	class Resource;
	class ResourceManager;
	class ResourceTypeInfo;
	class SceneDescription;
	class SceneManager;
} // namespace brk

namespace brk::editor::ui {
	struct BRK_EDITOR_API UiData
	{
		void Display(
			ecs::EntityWorld& world,
			ResourceManager& resourceManager,
			SceneManager& sceneManager);

		void MenuBar();
		void StartupWindow();
		void SceneSelection(const TULIDMap<SceneDescription>& sceneDescriptions);
		bool SceneCreation();

		bool Outliner(SceneManager& sceneManager);
		bool Inspector(ecs::EntityWorld& entityWorld, SceneManager& sceneManager);

		void ResourceExplorer(const ResourceManager& resourceManager);
		Resource* ResourceCreationWindow(ResourceManager& resourceManager);
		void ResourceEditor();

		void OpenSceneCreationWindow();

		bool m_ShowStartupWindow = false;
		bool m_ShowSceneCreationWindow = false;
		bool m_ShowOutliner = false;
		bool m_ShowResourceCreationWindow = false;

		bool m_ProjectLoadRequested = false;
		bool m_SceneLoadRequested = false;
		bool m_NewSceneRequested = false;
		bool m_AddResourceRequested = false;

		const char* m_FilePath = nullptr;

		struct {
			ULID m_SelectedObjectId;
			bool m_CreateObjectRequested = false;
			bool m_ShowObjectDeleteWarning = false;
			bool m_DeleteObjectRequested = false;
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
			bool m_ShowDeletionWarning = false;
			bool m_DeletionRequested = false;
		} m_ResourceEditorData;

		struct ModalPopup
		{
			bool m_Show = false;
			const char* m_Title = nullptr;
			std::string m_Content;
			void Open(const char* title, std::string content);
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