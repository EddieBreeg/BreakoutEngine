#include "SceneManager.hpp"

#include <core/Assert.hpp>
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <core/ULIDFormatter.hpp>
#include <ecs/ComponentRegistry.hpp>
#include "ECSManager.hpp"

#ifdef BRK_EDITOR
#include <fstream>
#include <nlohmann/json.hpp>

namespace {
	brk::ecs::GameObject LoadGameObject(const nlohmann::json& json, entt::registry& world)
	{
		brk::ecs::GameObject object;

		if (!Visit("id", json, object.m_Id))
		{
			BRK_LOG_WARNING("Game object doesn't have a ULID");
			return object;
		}
		DEBUG_CHECK(object.m_Id)
		{
			BRK_LOG_WARNING("Game object doesn't have a valid ULID");
			return object;
		}
		object.m_Entity = world.create();

		auto it = json.find("components");
		if (it == json.end())
			return object;

		auto& registry = brk::ecs::ComponentRegistry::GetInstance();

		for (auto comp : it->items())
		{
			const brk::ecs::ComponentInfo& info = registry.GetInfo(comp.key());
			object.m_Components.emplace_back(&info);
			info.m_LoadJson(comp.value(), world, object.m_Entity);
		}
		return object;
	}
} // namespace

void brk::SceneManager::LoadSceneDescriptions(const nlohmann::json& descriptions)
{
	BRK_ASSERT(
		descriptions.is_array(),
		"Tried to load scene descriptions from non-array object");

	for (const nlohmann::json data : descriptions)
	{
		SceneDescription desc;
		if(!JsonLoader<SceneDescription>::Load(desc, data))
		{
			BRK_LOG_WARNING("Failed to load description for scene '{}'", desc.GetName());
			continue;
		}

		BRK_ASSERT(desc.GetId(), "Scene '{}' has invalid ULID", desc.GetName());
		BRK_ASSERT(
			desc.GetName().length(),
			"Scene '{}' doesn't have a name",
			desc.GetId());
		m_Descriptions.emplace(desc.GetId(), std::move(desc));
	}
}
#endif

void brk::SceneManager::LoadScene(const ULID id)
{
	const auto it = m_Descriptions.find(id);
	DEBUG_CHECK(it != m_Descriptions.end())
	{
		BRK_LOG_WARNING("Couldn't load scene '{}': id not found", id);
		return;
	}
#ifdef BRK_DEV
	const std::string& filePath = it->second.GetPath();
	std::ifstream file{ filePath };
	if (!file.is_open())
	{
		BRK_LOG_WARNING("Couldn't open scene file '{}'", filePath);
		return;
	}
	nlohmann::json json = nlohmann::json::parse(file);
	std::vector<nlohmann::json> objects;

	if(!Visit("objects", json, objects))
		return;

	entt::registry& world = ecs::Manager::GetInstance().GetWorld();
	for (const nlohmann::json& objectDesc : objects)
	{
		ecs::GameObject object = LoadGameObject(objectDesc, world);
		m_Objects.emplace(object.m_Id, std::move(object));
	}
#endif
}

const brk::ecs::GameObject* brk::SceneManager::GetObject(const ULID id) const
{
	const auto it = m_Objects.find(id);
	return it == m_Objects.end() ? nullptr : &it->second;
}
