#include "ResourceManager.hpp"
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <core/ULIDFormatter.hpp>
#include <entt/entity/registry.hpp>

std::unique_ptr<brk::ResourceManager> brk::ResourceManager::s_Instance;

brk::ResourceManager::~ResourceManager()
{
	std::unique_lock lock{ m_Mutex };
	for (auto&& [id, resource] : m_Resources)
	{
		resource->MarkForDeletion();
	}

	for (auto&& [hash, info] : m_TypeMap)
	{
		info->m_Pool.Reset();
	}
#ifdef BRK_DEV
	const auto& allocInfo = m_AllocTracker.GetInfo();
	if (allocInfo.m_TotalSize)
	{
		BRK_LOG_WARNING(
			"Resource manager is still using {} bytes on exit, check for leaks!",
			allocInfo.m_TotalSize);
	}
#endif
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

		{
			std::shared_lock lock{ m_Mutex };
			if (m_Resources.find(resId) != m_Resources.end())
				continue;
		}

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
		ResourceTypeInfo& info = *it->second;

		Resource* res = info.NewResource(resId);

		if (!JsonLoader<Resource>::Load(*res, desc))
		{
			BRK_LOG_WARNING("Loading resource '{}' failed", res->GetId());
			res->MarkForDeletion();
			continue;
		}
		if (info.m_Load && !info.m_Load(*res, desc))
		{
			BRK_LOG_WARNING("Loading resource '{}' failed", res->GetId());
			res->MarkForDeletion();
			continue;
		}
		std::unique_lock lock{ m_Mutex };
		m_Resources.emplace(res->GetId(), res);
	}
}
#endif

brk::ResourceTypeInfo& brk::ResourceManager::GetResourceTypeInfo(
	StringView typeName) const
{
	const uint32 hash = Hash<StringView>{}(typeName);
	const auto it = m_TypeMap.find(hash);
	BRK_ASSERT(
		it != m_TypeMap.end(),
		"Tried resource type info for {}, which hasn't been registered");
	return *it->second;
}

void brk::ResourceManager::AddResource(Resource* res)
{
	BRK_ASSERT(res, "Null pointer passed to AddResource");
	const ULID id = res->GetId();
	BRK_ASSERT(id, "Tried to add resource with invalid ULID");
	std::unique_lock lock{ m_Mutex };
	BRK_ASSERT(
		m_Resources.try_emplace(id, res).second,
		"Couldn't add resource {} to the manager: ID already present",
		id);
}

void brk::ResourceManager::DeleteResource(Resource* res)
{
	res->MarkForDeletion();
	std::unique_lock lock{ m_Mutex };
	m_Resources.erase(res->GetId());
}

brk::ResourceManager::ResourceManager(entt::registry& world) noexcept
	: m_World{ world }
{}
