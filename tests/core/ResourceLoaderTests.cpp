#include <core/ResourceLoader.hpp>
#include <core/Resource.hpp>
#include <memory_resource>

namespace brk::resource_loader::ut {
	struct Res : public Resource
	{
		Res(const ULID id)
			: Resource(id)
		{}

		Res(EStateFlags state)
			: Resource(ULID::Generate())
		{
			m_State = state;
		}

		ResourceTypeInfo& GetTypeInfo() const override;

		bool DoLoad() override
		{
			if (GetLoadingState() != Loaded)
				++m_LoadCount;
			return true;
		}
		void SetLoadState(Resource::EStateFlags state) { m_State = state; }

		uint32 m_LoadCount = 0;
	};

	struct RAIIHelper
	{
		RAIIHelper()
			: m_Loader{ ResourceLoader::Init() }
		{
			ResourceTypeInfo::InitFor<Res, void>("res");
		}

		~RAIIHelper()
		{
			ResourceLoader::Reset();
			ResourceTypeInfo::ResetFor<Res>();
		}

		ResourceLoader& m_Loader;
	};

	void Tests()
	{
		{
			RAIIHelper helper;
			helper.m_Loader.ProcessBatch();
			helper.m_Loader.Wait();
		}
		{
			RAIIHelper helper;
			Res r{ Resource::Loading };
			helper.m_Loader.AddJob(&r);
			helper.m_Loader.ProcessBatch();
			helper.m_Loader.Wait();
			assert(r.GetLoadingState() == Resource::Loaded);
		}
		{
			RAIIHelper helper;
			Res r{ Resource::Loading };
			Res r2{ Resource::Loading };
			helper.m_Loader.AddJob(&r);
			helper.m_Loader.ProcessBatch();
			helper.m_Loader.AddJob(&r2);

			helper.m_Loader.Wait();

			// here, only r should have been processed since r2 was queued after the call
			// to ProcessBatch
			assert(r.GetLoadingState() == Resource::Loaded);
			assert(r2.GetLoadingState() == Resource::Loading);
		}
		{
			RAIIHelper helper;
			Res r{ Resource::Loading };
			helper.m_Loader.AddJob(&r);
			r.SetLoadState(Resource::Unloading);
			helper.m_Loader.AddJob(&r, ResourceLoader::EJobType::Unload);

			helper.m_Loader.ProcessBatch();
			helper.m_Loader.Wait();
			assert(r.GetLoadingState() == Resource::Unloaded);
		}
		{
			RAIIHelper helper;
			Res r{ Resource::Reloading };
			helper.m_Loader.AddJob(&r, ResourceLoader::EJobType::Reload);
			helper.m_Loader.ProcessBatch();
			helper.m_Loader.Wait();
			assert(r.m_LoadCount == 1);
		}
	}
} // namespace brk::resource_loader::ut

namespace brk {
	RES_INFO_IMPL_NO_ATTR(resource_loader::ut::Res);
}