#include "ResourceRef.hpp"

brk::ResourceLoadingRequests brk::ResourceLoadingRequests::s_Instance;

void brk::RetainTraits<brk::Resource>::Increment(Resource* res)
{
	if (++res->m_RefCount == 1 && res->m_LoadingState != Resource::Loading &&
		res->m_LoadingState != Resource::Loaded)
	{
		res->m_LoadingState = Resource::Loading;
		ResourceLoadingRequests::s_Instance.m_LoadRequests.emplace_back(res);
	}
}

void brk::RetainTraits<brk::Resource>::Decrement(Resource* res)
{
	if (!--res->m_RefCount && res->m_LoadingState != Resource::Unloaded &&
		res->m_LoadingState != Resource::Unloading)
	{
		res->m_LoadingState = Resource::Unloading;
		ResourceLoadingRequests::s_Instance.m_UnloadRequests.emplace_back(res);
	}
}

uint32 brk::RetainTraits<brk::Resource>::GetCount(const Resource* res)
{
	return res->m_RefCount;
}