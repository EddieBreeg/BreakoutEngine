#include <core/TimeInfo.hpp>
#include <ecs/World.hpp>
#include <managers/ECSManager.hpp>
#include <cassert>

namespace brk::ecs::ut::manager {
	struct TestComponent
	{};

	struct System : Singleton<System>
	{
		using World = WorldView<TestComponent>;

		void Update(World& world, const TimeInfo& time)
		{
			world.AddComponent<TestComponent>(world.CreateEntity());
		}
	};

	struct RAIIHelper
	{
		RAIIHelper()
			: m_Manager{ ecs::Manager::Init() }
			, m_EntityWorld{ m_Manager.GetWorld() }
		{}

		~RAIIHelper() { ecs::Manager::Reset(); }

		ecs::Manager& m_Manager;
		entt::registry& m_EntityWorld;
		TimeInfo m_Time;
	};

	void AddComponentTest()
	{
		using namespace brk::ecs::ut::manager;
		RAIIHelper helper;
		helper.m_Manager.AddSystem<System>();
		helper.m_Manager.Update(helper.m_Time);

		using TQuery = brk::ecs::query::Include<const TestComponent>;
		using TWorld = brk::ecs::WorldView<const TestComponent>;

		bool found = false;
		for (const entt::entity e : TWorld{ helper.m_EntityWorld }.Query<TQuery>())
		{
			found = true;
			break;
		}
		assert(found);
	}

	void DoNothingTest()
	{
		RAIIHelper helper;
		using TQuery = brk::ecs::query::Include<const TestComponent>;
		using TWorld = brk::ecs::WorldView<const TestComponent>;

		helper.m_Manager.Update(helper.m_Time);

		bool found = false;
		for (const entt::entity e : TWorld{ helper.m_EntityWorld }.Query<TQuery>())
		{
			found = true;
			break;
		}
		assert(!found);
	}
} // namespace brk::ecs::ut::manager
