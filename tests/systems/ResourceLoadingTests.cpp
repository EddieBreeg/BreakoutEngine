#include <core/Resource.hpp>
#include <core/TimeInfo.hpp>
#include <managers/ECSManager.hpp>
#include <systems/ResourceLoadingSystem.hpp>

namespace brk::resource_loading::ut {
	struct Res : public Resource
	{
		bool DoLoad() override
		{
			m_LoadingState = Loaded;
			return true;
		}
		void SetLoadingState(ELoadingState s) { m_LoadingState = s; }
	};

	struct RAIIHelper
	{
		RAIIHelper()
			: m_Manager{ ecs::Manager::Init() }
			, m_EntityWorld{ m_Manager.GetWorld() }
		{
			m_Manager.AddSystem<ResourceLoadingSystem>();
		}

		~RAIIHelper() { m_Manager.Reset(); }

		TimeInfo m_Time;
		ecs::Manager& m_Manager;
		entt::registry& m_EntityWorld;
	};

	void ResourceLoadingTests()
	{
		{
			RAIIHelper helper;
			Res r;
			helper.m_Manager.Update(helper.m_Time);
			assert(r.GetLoadingState() == Resource::Unloaded);
		}
		{
			RAIIHelper helper;
			Res r;
			ResourceLoadingRequests::s_Instance.m_LoadRequests.emplace_back(&r);
			r.SetLoadingState(Resource::Loading);
			helper.m_Manager.Update(helper.m_Time);
			assert(r.GetLoadingState() == Resource::Loaded);

			r.SetLoadingState(Resource::Unloading);
			ResourceLoadingRequests::s_Instance.m_UnloadRequests.emplace_back(&r);

			helper.m_Manager.Update(helper.m_Time);
			assert(r.GetLoadingState() == Resource::Unloaded);
		}
		{
			RAIIHelper helper;
			Res r;
			ResourceLoadingRequests::s_Instance.m_LoadRequests.emplace_back(&r);

			r.SetLoadingState(Resource::Loading);
			helper.m_Manager.Update(helper.m_Time);
			assert(r.GetLoadingState() == Resource::Loaded);

			ResourceLoadingRequests::s_Instance.m_LoadRequests.emplace_back(&r);
			helper.m_Manager.Update(helper.m_Time);
			assert(r.GetLoadingState() == Resource::Loaded);
		}
		{
			RAIIHelper helper;
			Res r;
			r.DoLoad();
			ResourceLoadingRequests::s_Instance.m_UnloadRequests.emplace_back(&r);
			helper.m_Manager.Update(helper.m_Time);
			// unloading request should be ignored if loading state is not
			// Resource::Unloading
			assert(r.GetLoadingState() == Resource::Loaded);
		}
		{
			RAIIHelper helper;
			Res r;
			ResourceLoadingRequests::s_Instance.m_LoadRequests.emplace_back(&r);
			r.SetLoadingState(Resource::Unloading);
			helper.m_Manager.Update(helper.m_Time);
			// if we "change our mind" and don't need the request after all, then cancel
			// loading
			assert(r.GetLoadingState() != Resource::Loaded);
		}
		{
			RAIIHelper helper;
			Res r;
			r.SetLoadingState(Resource::Loading);
			ResourceLoadingRequests::s_Instance.m_LoadRequests.emplace_back(&r);
			ResourceLoadingRequests::s_Instance.m_UnloadRequests.emplace_back(&r);
			helper.m_Manager.Update(helper.m_Time);
			assert(r.GetLoadingState() == Resource::Loaded);
		}
	}
} // namespace brk::resource_loading::ut
