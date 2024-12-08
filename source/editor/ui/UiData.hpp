#pragma once

#include <PCH.hpp>

#ifdef BRK_EDITOR
#include <core/ULID.hpp>
#include <entt/entity/fwd.hpp>

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
			entt::registry& world,
			ResourceManager& resourceManager,
			SceneManager& sceneManager);

		void MenuBar();
		void StartupWindow();
		void SceneSelection(const TULIDMap<SceneDescription>& sceneDescriptions);
		bool SceneCreation();

		bool Outliner(SceneManager& sceneManager);
		bool Inspector(entt::registry& entityWorld, SceneManager& sceneManager);

		void ResourceExplorer();
		Resource* ResourceCreationWindow(ResourceManager& resourceManager);

		void OpenSceneCreationWindow();

		bool m_ShowStartupWindow = false;
		bool m_ShowSceneCreationWindow = false;
		bool m_ShowOutliner = false;
		bool m_ShowResourceCreationWindow = false;

		bool m_ProjectLoadRequested = false;
		bool m_SceneLoadRequested = false;
		bool m_NewSceneRequested = false;

		const char* m_FilePath = nullptr;

		struct
		{
			const ResourceTypeInfo* m_Info = nullptr;
			Resource* m_Resource = nullptr;
		} m_ResourceCreationData;

		ULID m_SceneId;
		ULID m_SelectedObjectId;
	};

} // namespace brk::editor::ui
#endif