#pragma once

#include <PCH.hpp>
#include <core/FieldList.hpp>

#include <string>
#include <filesystem>

namespace brk
{
	template<class T, class>
	struct JsonLoader;
}

namespace brk::editor {
	class Project
	{
	public:
		Project() = default;
		Project(const Project&) = delete;
		Project(Project&&) = default;
		~Project() = default;

		Project& operator=(Project&&) = default;

		[[nodiscard]] const std::string& GetName() const noexcept { return m_Name; }

	private:
		std::filesystem::path m_ProjectDir;
		std::string m_Name;
		friend class Editor;
		friend struct JsonLoader<Project, void>;

	public:
		static constexpr meta::FieldList<&Project::m_Name> Fields{
			"name",
		};
	};
} // namespace brk::editor
