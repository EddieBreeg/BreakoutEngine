#include "ResourceLoadingSystem.hpp"
#include "ResourceLoadingComponents.hpp"
#include <core/Resource.hpp>

void brk::ResourceLoadingSystem::Update(World& world, const TimeInfo&)
{
	auto loadRequestsWorld =
		world.Query<ecs::query::Include<const ResourceLoadRequestComponent>>();
	for (const entt::entity e : loadRequestsWorld)
	{
		const auto& req = loadRequestsWorld.Get<const ResourceLoadRequestComponent>(e);
		if (req.m_Res->DoLoad())
			req.m_Res->m_LoadingState = Resource::Loaded;
		world.DestroyEntity(e);
	}

	auto unloadRequestsWorld =
		world.Query<ecs::query::Include<const ResourceUnloadRequestComponent>>();
	for (const entt::entity e : unloadRequestsWorld)
	{
		const auto& req = unloadRequestsWorld.Get<const ResourceUnloadRequestComponent>(e);
		req.m_Res->DoUnload();
		req.m_Res->m_LoadingState = Resource::Unloaded;
		world.DestroyEntity(e);
	}
}
