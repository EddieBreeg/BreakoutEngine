#pragma once

#include <PCH.hpp>
#ifdef BRK_EDITOR

#include <core/Singleton.hpp>
#include <core/ULID.hpp>
#include "Project.hpp"
#include <optional>

struct ImGuiContext;

namespace brk {
	class ResourceManager;
	class SceneManager;
} // namespace brk

namespace brk::ecs {
	class Manager;
} // namespace brk::ecs

namespace brk::editor::ui {
	struct UiData;
}

namespace brk::editor {
	class BRK_EDITOR_API Editor : public Singleton<Editor>
	{
	public:
		~Editor() = default;

		void Update();
		void ShowUI();

	private:
		friend class Singleton<Editor>;
		static std::unique_ptr<Editor> s_Instance;
		Editor(
			ImGuiContext& imguICtx,
			ecs::Manager& ecsManager,
			ResourceManager& resManager,
			SceneManager& sceneManager,
			int argc,
			const char** argv);

		void LoadProject(const char* path);
		void LoadScene(ULID id);
		void CreateNewScene(const char* path);

		void SaveProjectFile();

		ui::UiData* m_UiData = nullptr;
		std::string m_ProjectFilePath;
		std::optional<Project> m_Project;

		ecs::Manager& m_ECSManager;
		ResourceManager& m_ResourceManager;
		SceneManager& m_SceneManager;
	};
} // namespace brk::editor

#endif