#include "ResourceRef.hpp"
#include "ResourceLoader.hpp"

void brk::RetainTraits<brk::Resource>::Increment(Resource* res)
{
	if (++res->m_RefCount == 1 && res->m_LoadingState != Resource::Loading &&
		res->m_LoadingState != Resource::Loaded)
	{
		res->m_LoadingState = Resource::Loading;
		ResourceLoader::GetInstance().AddJob(res);
	}
}

void brk::RetainTraits<brk::Resource>::Decrement(Resource* res)
{
	if (!--res->m_RefCount && res->m_LoadingState != Resource::Unloaded &&
		res->m_LoadingState != Resource::Unloading)
	{
		res->m_LoadingState = Resource::Unloading;
		ResourceLoader::GetInstance().AddJob(res, false);
	}
}

uint32 brk::RetainTraits<brk::Resource>::GetCount(const Resource* res)
{
	return res->m_RefCount;
}