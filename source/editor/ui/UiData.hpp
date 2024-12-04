#pragma once

#include <PCH.hpp>

#ifdef BRK_EDITOR
#include <core/ULID.hpp>
#include <entt/entity/fwd.hpp>

namespace brk {
	class SceneDescription;
	class SceneManager;
} // namespace brk

namespace brk::editor::ui {
	struct BRK_EDITOR_API UiData
	{
		void Display(entt::registry& world, SceneManager& sceneManager);

		void MenuBar();
		void StartupWindow();
		void SceneSelection(const TULIDMap<SceneDescription>& sceneDescriptions);
		bool SceneCreation();

		bool Outliner(SceneManager& sceneManager);
		bool Inspector(entt::registry& entityWorld, SceneManager& sceneManager);

		void OpenSceneCreationWindow();

		bool m_ShowStartupWindow = false;
		bool m_ShowSceneSelectionWindow = false;
		bool m_ShowSceneCreationWindow = false;
		bool m_ShowOutliner = false;

		bool m_NewSceneRequested = false;

		const char* m_FilePath = nullptr;

		ULID m_SelectedObjectId;
	};

} // namespace brk::editor::ui
#endif