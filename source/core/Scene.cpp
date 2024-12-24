#include "Scene.hpp"

namespace brk {
#ifdef BRK_DEV
	SceneDescription::SceneDescription(std::string name, std::string file)
		: m_Id{ ULID::Generate() }
		, m_Name{ std::move(name) }
		, m_File{ std::move(file) }
	{}
#endif
} // namespace brk
