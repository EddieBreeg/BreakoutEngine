#include "ResourceLoadingSystem.hpp"
#include <core/Resource.hpp>

brk::ResourceLoadingRequests brk::ResourceLoadingRequests::s_Instance;

void brk::ResourceLoadingSystem::Update(World& world, const TimeInfo&)
{
	ResourceLoadingRequests& requests = ResourceLoadingRequests::s_Instance;

	for (Resource* res : requests.m_LoadRequests)
	{
		if (res->GetLoadingState() != Resource::Loading)
			continue;
		if (res->DoLoad())
			res->m_LoadingState = Resource::Loaded;
	}

	for (Resource* res : requests.m_UnloadRequests)
	{
		if (res->GetLoadingState() != Resource::Unloading)
			continue;
		res->DoUnload();
		res->m_LoadingState = Resource::Unloaded;
	}

	requests.m_LoadRequests.clear();
	requests.m_UnloadRequests.clear();
}
