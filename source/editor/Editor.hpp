#pragma once

#include <PCH.hpp>
#include <core/Singleton.hpp>
#include "Project.hpp"
#include <optional>

#ifdef BRK_EDITOR

namespace brk::editor {
	class Editor : public Singleton<Editor>
	{
	public:
		~Editor() = default;

		void Update();
		void ShowUI();

	private:
		friend class Singleton<Editor>;
		Editor(int argc, const char** argv);

		void OnProjectFilePathChanged();

		std::string_view m_ProjectFilePath;
		std::optional<Project> m_Project;
	};
} // namespace brk

#endif