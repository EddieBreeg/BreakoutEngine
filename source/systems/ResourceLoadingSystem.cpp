#include "ResourceLoadingComponents.hpp"
#include "ResourceLoadingSystem.hpp"
#include <core/Resource.hpp>

void brk::ResourceLoadingSystem::Update(World& world, const TimeInfo&)
{
	auto loadRequestsWorld =
		world.Query<ecs::query::Include<const ResourceLoadRequestComponent>>();
	for (const entt::entity e : loadRequestsWorld)
	{
		const auto& req = loadRequestsWorld.Get<const ResourceLoadRequestComponent>(e);
		req.m_Res->m_IsLoaded = req.m_Res->DoLoad();
		world.DestroyEntity(e);
	}

	auto unloadRequestsWorld =
		world.Query<ecs::query::Include<const ResourceUnloadRequestComponent>>();
	for (const entt::entity e : unloadRequestsWorld)
	{
		const auto& req = unloadRequestsWorld.Get<const ResourceUnloadRequestComponent>(e);
		req.m_Res->DoUnload();
		req.m_Res->m_IsLoaded = false;
		world.DestroyEntity(e);
	}
}
