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
	if (--res->m_RefCount)
		return;

	switch (res->GetLoadingState())
	{
	case Resource::MarkedForDeletion: delete res; return;
	case Resource::Loaded:
	case Resource::Loading:
		res->SetLoadingState(Resource::Unloading);
		ResourceLoader::GetInstance().AddJob(res, ResourceLoader::EJobType::Unload);
		return;
	default: break;
	}
}

uint32 brk::RetainTraits<brk::Resource>::GetCount(const Resource* res)
{
	return res->m_RefCount;
}