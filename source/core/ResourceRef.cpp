#include "ResourceRef.hpp"
#include "ResourceLoader.hpp"

void brk::RetainTraits<brk::Resource>::Increment(Resource* res)
{
	const Resource::EStateFlags loadState = res->GetLoadingState();
	if (++res->m_RefCount == 1 && loadState != Resource::Loading &&
		loadState != Resource::Loaded)
	{
		res->SetLoadingState(Resource::Loading);
		ResourceLoader::GetInstance().AddJob(res);
	}
}

void brk::RetainTraits<brk::Resource>::Decrement(Resource* res)
{
	const Resource::EStateFlags loadState = res->GetLoadingState();
	if (!--res->m_RefCount && loadState != Resource::Unloaded &&
		loadState != Resource::Unloading)
	{
		res->SetLoadingState(Resource::Unloading);
		ResourceLoader::GetInstance().AddJob(res, false);
	}
}

uint32 brk::RetainTraits<brk::Resource>::GetCount(const Resource* res)
{
	return res->m_RefCount;
}