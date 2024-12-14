#include <core/ResourceLoader.hpp>
#include <core/Resource.hpp>

namespace brk::resource_loader::ut {
	struct RAIIHelper
	{
		RAIIHelper()
			: m_Loader{ ResourceLoader::Init() }
		{}

		~RAIIHelper() { ResourceLoader::Reset(); }

		ResourceLoader& m_Loader;
	};

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
		static inline const ResourceTypeInfo Info = ResourceTypeInfo::Create<Res>("res");
		const ResourceTypeInfo& GetTypeInfo() const noexcept override { return Info; }

		bool DoLoad() override
		{
			if (GetLoadingState() != Loaded)
				++m_LoadCount;
			return true;
		}
		void SetLoadState(Resource::EStateFlags state) { m_State = state; }

		uint32 m_LoadCount = 0;
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
