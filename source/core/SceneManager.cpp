#include "SceneManager.hpp"

#include "Assert.hpp"
#include "Loaders.hpp"
#include "LogManager.hpp"

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
		DEBUG_CHECK(desc.GetId())
		{
			LogManager::GetInstance().Log(
				LogManager::Warning,
				"Failed to load scene description for scene '{}': invalid ULID",
				desc.GetPath());
			continue;
		}
		m_Descriptions.emplace(desc.GetId(), std::move(desc));
	}
}
#endif