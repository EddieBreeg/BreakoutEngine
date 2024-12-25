#pragma once

#include <PCH.hpp>
#ifdef BRK_EDITOR

#include <core/Singleton.hpp>
#include <core/ULID.hpp>
#include "Project.hpp"
#include <optional>

struct ImGuiContext;
struct ImGuiIO;

namespace brk {
	class ResourceManager;
	class SceneManager;
} // namespace brk

namespace brk::ecs {
	class Manager;
} // namespace brk::ecs

namespace brk::editor::ui {
	struct UiData;

	BRK_EDITOR_API void ImportEditorFonts(ImGuiIO& io, float dpiScale);
} // namespace brk::editor::ui

namespace brk::editor {
	class Editor : public Singleton<Editor>
	{
	public:
		~Editor() = default;

		BRK_EDITOR_API void Update();
		BRK_EDITOR_API void ShowUI();

	private:
		friend class Singleton<Editor>;

		BRK_EDITOR_API static inline std::unique_ptr<Editor> s_Instance;

		BRK_EDITOR_API Editor(
			ImGuiContext& imguICtx,
			ecs::Manager& ecsManager,
			ResourceManager& resManager,
			SceneManager& sceneManager,
			int argc,
			const char** argv);

		void LoadProject(const std::filesystem::path filePath);
		void LoadScene(ULID id);

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