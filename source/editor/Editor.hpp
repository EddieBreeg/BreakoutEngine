#pragma once

#include <PCH.hpp>
#ifdef BRK_EDITOR

#include <core/Singleton.hpp>
#include <core/ULID.hpp>
#include "Project.hpp"
#include <optional>

namespace brk {
	class App;
	class SceneManager;
} // namespace brk

namespace brk::ecs {
	class Manager;
} // namespace brk::ecs

namespace brk::editor {
	class BRK_EDITOR_API Editor : public Singleton<Editor>
	{
	public:
		~Editor() = default;

		void LoadProjectDeferred(const StringView filePath) noexcept;
		void LoadSceneDeferred(const ULID sceneId) noexcept;
		void Update();
		void ShowUI();

	private:
		friend class Singleton<Editor>;
		static std::unique_ptr<Editor> s_Instance;
		Editor(
			App& app,
			ecs::Manager& ecsManager,
			SceneManager& sceneManager,
			int argc,
			const char** argv);

		enum class LoadState
		{
			None = 0,
			Project,
			Scene,
		} m_LoadState = LoadState::None;

		void LoadProject();
		void LoadScene();
		void CreateNewScene(const char* path);

		void SaveProjectFile();

		std::string m_ProjectFilePath;
		std::optional<Project> m_Project;
		ULID m_CurrentScene;

		ecs::Manager& m_ECSManager;
		SceneManager& m_SceneManager;

		bool m_NewSceneRequested = false;
	};
} // namespace brk::editor

#endif