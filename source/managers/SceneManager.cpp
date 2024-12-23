#include "SceneManager.hpp"

#include <core/Assert.hpp>
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <core/StringViewFormatter.hpp>
#include <ecs/ComponentRegistry.hpp>
#include <entt/entity/registry.hpp>
#include "ECSManager.hpp"

#ifdef BRK_DEV
#include <core/ULIDFormatter.hpp>
#include <fmt/format.h>
#include <fstream>
#include <nlohmann/json.hpp>

namespace {
	brk::ecs::GameObject LoadGameObject(
		const nlohmann::json& json,
		brk::ecs::EntityWorld& world)
	{
		brk::ecs::GameObject object;

		if (!brk::Visit("id", json, object.m_Id))
		{
			BRK_LOG_WARNING("Game object doesn't have a ULID");
			return object;
		}
		DEBUG_CHECK(object.m_Id)
		{
			BRK_LOG_WARNING("Game object doesn't have a valid ULID");
			return object;
		}

		if (!brk::Visit("name", json, object.m_Name))
		{
			BRK_LOG_WARNING("Game object {} doesn't have a name", object.m_Id);
		}

		object.m_Entity = world.create();

		auto it = json.find("components");
		if (it == json.end())
			return object;

		auto& registry = brk::ecs::ComponentRegistry::GetInstance();

		for (auto& comp : it->items())
		{
			const brk::ecs::ComponentInfo& info = registry.GetInfo(comp.key());
			const void* componentPtr =
				info.m_LoadJson(comp.value(), world, object.m_Entity);
			if (!componentPtr)
			{
				BRK_LOG_WARNING(
					"Failed to load component {} for object {}",
					info.m_Name,
					object.m_Name);
				continue;
			}
			void* widget = info.m_WidgetInfo.m_Create
							   ? info.m_WidgetInfo.m_Create(componentPtr)
							   : nullptr;
			object.m_Components.emplace_back(info, widget);
		}
		return object;
	}
} // namespace

std::unique_ptr<brk::SceneManager> brk::SceneManager::s_Instance;

void brk::SceneManager::LoadSceneDescriptions(const nlohmann::json& descriptions)
{
	BRK_ASSERT(
		descriptions.is_array(),
		"Tried to load scene descriptions from non-array object");

	for (const nlohmann::json data : descriptions)
	{
		SceneDescription desc;
		if (!JsonLoader<SceneDescription>::Load(desc, data))
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

const brk::SceneDescription& brk::SceneManager::CreateNewScene(
	std::string name,
	std::string path)
{
	SceneDescription desc{ std::move(name), std::move(path) };
	m_CurrentSceneId = desc.GetId();
	return m_Descriptions.emplace(desc.GetId(), std::move(desc)).first->second;
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

	if (!Visit("objects", json, objects))
		return;

	ecs::EntityWorld& world = ecs::Manager::GetInstance().GetWorld();
	uint32 nObjects = 0;
	for (const nlohmann::json& objectDesc : objects)
	{
		ecs::GameObject object = LoadGameObject(objectDesc, world);
		m_Objects.emplace(object.m_Id, std::move(object));
		++nObjects;
	}
	BRK_LOG_TRACE("Loaded {} game objects from scene file", nObjects);
	m_CurrentSceneId = id;
#endif
}

void brk::SceneManager::ClearCurrentScene()
{
	m_Objects.clear();
	m_CurrentSceneId = {};
}

const brk::ecs::GameObject* brk::SceneManager::GetObject(const ULID id) const
{
	const auto it = m_Objects.find(id);
	return it == m_Objects.end() ? nullptr : &it->second;
}

brk::ecs::GameObject* brk::SceneManager::GetObject(const ULID id)
{
	const auto it = m_Objects.find(id);
	return it == m_Objects.end() ? nullptr : &it->second;
}

brk::ecs::GameObject& brk::SceneManager::CreateObject(ecs::EntityWorld& world)
{
	ecs::GameObject object{
		ULID::Generate(),
		fmt::format("Game Object {}", m_Objects.size()),
		world.create(),
	};
	return m_Objects.emplace(object.m_Id, std::move(object)).first->second;
}

void brk::SceneManager::DeleteObject(const ULID id)
{
	DEBUG_CHECK(m_Objects.erase(id))
	{
		BRK_LOG_WARNING("Tried to delete non-existent game object {}", id);
	}
}

void brk::SceneManager::SaveCurrentSceneToFile(const ecs::EntityWorld& world) const
{
	BRK_ASSERT(
		m_CurrentSceneId,
		"Called SaveCurrentSceneToFile() with no scene selected");

	const SceneDescription& desc = m_Descriptions.find(m_CurrentSceneId)->second;
	nlohmann::json json;
	json["id"] = m_CurrentSceneId;
	json["name"] = desc.GetName();
	auto& objectsArray = json["objects"] = nlohmann::json::array();

	nlohmann::json objJson;

	for (const auto& [id, object] : m_Objects)
	{
		objJson["id"] = id;
		objJson["name"] = object.m_Name;
		auto& components = objJson["components"] = nlohmann::json::object();

		for (const auto& [info, widget] : object.m_Components)
		{
			auto& tmp = components[info->m_Name] = nlohmann::json::object();
			info->m_SaveJson(tmp, world, object.m_Entity);
		}

		objectsArray.emplace_back(std::move(objJson));
	}

	std::ofstream file{ desc.GetPath() };
	BRK_ASSERT(
		file.is_open(),
		"Failed to save scene to {}: {}",
		desc.GetPath(),
		std::strerror(errno));
	file << json.dump(4);
}
