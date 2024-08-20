#include "Scene.hpp"

namespace brk {
	SceneDescription::SceneDescription(std::string name, std::string file)
		: m_Id{ ULID::Generate() }
		, m_Name{ name }
		, m_File{ file }
	{}
} // namespace brk
