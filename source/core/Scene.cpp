#include "Scene.hpp"

namespace brk {
	SceneDescription::SceneDescription(std::string name, std::string file)
		: m_Id{ ULID::Generate() }
		, m_Name{ std::move(name) }
		, m_File{ std::move(file) }
	{}
} // namespace brk
