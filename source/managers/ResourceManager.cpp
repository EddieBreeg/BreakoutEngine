#include "ResourceManager.hpp"
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <core/ULIDFormatter.hpp>
#include <entt/entity/registry.hpp>
#include <systems/ResourceLoadingComponents.hpp>
#include <systems/ResourceLoadingSystem.hpp>

brk::ResourceManager::~ResourceManager()
{
	for (auto&& [id, res] : m_Resources)
		delete res;
}

void brk::ResourceManager::CreateResources(const std::vector<nlohmann::json>& list)
{
	ULID resId;
	StringView resType;

	for (const nlohmann::json& desc : list)
	{
		if (!Visit("id", desc, resId))
		{
			BRK_LOG_WARNING("Found resource with no id");
			continue;
		}

		if (m_Resources.find(resId) != m_Resources.end())
			continue;

		if (!Visit("type", desc, resType))
		{
			BRK_LOG_WARNING("Resource '{}' doesn't have a type", resId);
			continue;
		}

		Resource* res = CreateResource(resType, resId);
		if (!JsonLoader<Resource>::Load(*res, desc))
		{
			BRK_LOG_WARNING("Loading resource '{}' failed", res->GetId());
			delete res;
			continue;
		}
		m_Resources.emplace(res->GetId(), res);
	}
}

brk::Resource* brk::ResourceManager::CreateResource(const StringView type, const ULID id)
	const
{
	const uint32 h = Hash<StringView>{}(type);
	const auto it = m_TypeMap.find(h);
	BRK_ASSERT(
		it != m_TypeMap.end(),
		"Failed to create resource: type '{}' hasn't been registed",
		type);
	return (it->second)(id);
}

void brk::ResourceManager::LoadDeferred(Resource* res)
{
	res->m_LoadingState = Resource::Loading;
	m_World.emplace<ResourceLoadRequestComponent>(m_World.create(), res);
}

void brk::ResourceManager::UnloadDeferred(Resource* res)
{
	m_World.emplace<ResourceUnloadRequestComponent>(m_World.create(), res);
}

brk::ResourceManager::ResourceManager(entt::registry& world) noexcept
	: m_World{ world }
{}
