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
		Res(EStateFlags state)
			: Resource(ULID::Generate())
		{
			m_LoadingState = state;
		}

		bool DoLoad() override { return true; }
		void SetLoadState(Resource::EStateFlags state) { m_LoadingState = state; }
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
			helper.m_Loader.AddJob(&r, false);

			helper.m_Loader.ProcessBatch();
			helper.m_Loader.Wait();
			assert(r.GetLoadingState() == Resource::Unloaded);
		}
	}
} // namespace brk::resource_loader::ut
