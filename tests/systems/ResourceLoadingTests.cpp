#include <core/Resource.hpp>
#include <core/TimeInfo.hpp>
#include <managers/ECSManager.hpp>
#include <systems/ResourceLoadingComponents.hpp>
#include <systems/ResourceLoadingSystem.hpp>

namespace brk::resource_loading::ut {
	struct Res : public Resource
	{
		bool DoLoad() override { return true; }
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
			helper.m_EntityWorld.emplace<ResourceLoadRequestComponent>(
				helper.m_EntityWorld.create(),
				static_cast<Resource*>(&r));
			helper.m_Manager.Update(helper.m_Time);
			assert(r.GetLoadingState() == Resource::Loaded);

			helper.m_EntityWorld.emplace<ResourceUnloadRequestComponent>(
				helper.m_EntityWorld.create(),
				&r);
			helper.m_Manager.Update(helper.m_Time);
			assert(r.GetLoadingState() == Resource::Unloaded);
		}
		{
			RAIIHelper helper;
			Res r;
			helper.m_EntityWorld.emplace<ResourceLoadRequestComponent>(
				helper.m_EntityWorld.create(),
				static_cast<Resource*>(&r));
			helper.m_Manager.Update(helper.m_Time);
			assert(r.GetLoadingState() == Resource::Loaded);

			helper.m_EntityWorld.emplace<ResourceLoadRequestComponent>(
				helper.m_EntityWorld.create(),
				static_cast<Resource*>(&r));
			helper.m_Manager.Update(helper.m_Time);
			assert(r.GetLoadingState() == Resource::Loaded);
		}
	}
} // namespace brk::resource_loading::ut
