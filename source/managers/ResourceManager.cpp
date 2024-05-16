#include "ResourceManager.hpp"
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <core/ULIDFormatter.hpp>

brk::ResourceManager::~ResourceManager()
{
	for (auto [id, res] : m_Resources)
		delete res;
}

void brk::ResourceManager::PreloadResources(const nlohmann::json& list)
{
	BRK_ASSERT(list.is_array(), "Trying to preload resources from a non-array object");
	for (const nlohmann::json& desc : list)
	{
		Resource* res = CreateResource(desc);
		if (!JsonLoader<Resource>::Load(*res, desc))
		{
			BRK_LOG_WARNING("Loading resource '{}' failed", res->GetId());
			delete res;
			continue;
		}
		m_Resources.emplace(res->GetId(), res);
	}
}

brk::Resource* brk::ResourceManager::CreateResource(const nlohmann::json& desc) const
{
	StringView typeName;
	{
		const auto it = desc.find("type");
		BRK_ASSERT(it != desc.end(), "Failed to create resource: no type name");
		it->get_to(typeName);
	}
	const uint32 h = Hash<StringView>{}(typeName);
	const auto it = m_TypeMap.find(h);
	BRK_ASSERT(
		it != m_TypeMap.end(),
		"Failed to create resource: type '{}' hasn't been registed",
		typeName);
	return (it->second)();
}

void brk::ResourceManager::UnloadDeferred(Resource* res) {}
