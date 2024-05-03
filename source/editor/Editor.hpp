#pragma once

#include <PCH.hpp>
#include <core/Singleton.hpp>
#include <core/ULID.hpp>
#include "Project.hpp"
#include <optional>

#ifdef BRK_EDITOR

namespace brk::editor {
	class Editor : public Singleton<Editor>
	{
	public:
		~Editor() = default;

		void LoadProjectDeferred(const std::string_view filePath) noexcept;
		void LoadSceneDeferred(const ULID sceneId) noexcept;
		void Update();
		void ShowUI();

	private:
		friend class Singleton<Editor>;
		Editor(int argc, const char** argv);

		enum class LoadState
		{
			None = 0,
			Project,
			Scene,
		} m_LoadState = LoadState::None;

		void LoadProject();
		void LoadScene();

		std::string_view m_ProjectFilePath;
		std::optional<Project> m_Project;
		ULID m_CurrentScene;
	};
} // namespace brk::editor

#endif