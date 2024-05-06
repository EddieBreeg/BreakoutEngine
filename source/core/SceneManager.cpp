#include "SceneManager.hpp"

#include "Assert.hpp"
#include "Loaders.hpp"
#include "LogManager.hpp"
#include "ULIDFormatter.hpp"

#ifdef BRK_EDITOR
#include <nlohmann/json.hpp>

void brk::SceneManager::LoadSceneDescriptions(const nlohmann::json& descriptions)
{
	BRK_ASSERT(
		descriptions.is_array(),
		"Tried to load scene descriptions from non-array object");

	for (const nlohmann::json data : descriptions)
	{
		SceneDescription desc;
		JsonLoader<SceneDescription>::Load(desc, data);
		BRK_ASSERT(desc.GetId(), "Scene '{}' has invalid ULID", desc.GetName());
		BRK_ASSERT(
			desc.GetName().length(),
			"Scene '{}' doesn't have a name",
			desc.GetId());
		m_Descriptions.emplace(desc.GetId(), std::move(desc));
	}
}
#endif