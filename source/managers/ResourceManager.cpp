#include "ResourceManager.hpp"
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <core/ULIDFormatter.hpp>
#include <entt/entity/registry.hpp>

std::unique_ptr<brk::ResourceManager> brk::ResourceManager::s_Instance;

brk::ResourceManager::~ResourceManager()
{
	while (!m_Resources.empty())
	{
		for (auto it = m_Resources.begin(); it != m_Resources.end();)
		{
			if (it->second->GetRefCount())
			{
				++it;
				continue;
			}
			delete it->second;
			it = m_Resources.erase(it);
		}
	}
}

#ifdef BRK_EDITOR
void brk::ResourceManager::CreateResources(const nlohmann::json& list)
{
	ULID resId;
	StringView resType;
	BRK_ASSERT(
		list.is_array(),
		"CreateResources called with a non array type ({})",
		list.type_name());

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
		auto it = m_TypeMap.find(Hash<StringView>{}(resType));
		BRK_ASSERT(
			it != m_TypeMap.end(),
			"Attempted to load resource of unregistered type {}",
			resType);
		const ResourceTypeInfo& info = it->second;

		Resource* res = info.m_Constructor(resId);

		if (!JsonLoader<Resource>::Load(*res, desc))
		{
			BRK_LOG_WARNING("Loading resource '{}' failed", res->GetId());
			delete res;
			continue;
		}
		if (info.m_Load && !info.m_Load(*res, desc))
		{
			BRK_LOG_WARNING("Loading resource '{}' failed", res->GetId());
			delete res;
			continue;
		}
		m_Resources.emplace(res->GetId(), res);
	}
}
#endif

brk::ResourceManager::ResourceManager(entt::registry& world) noexcept
	: m_World{ world }
{}
