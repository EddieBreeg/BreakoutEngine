#include <cassert>
#include <core/Loaders.hpp>
#include <ecs/ComponentRegistry.hpp>
#include <entt/entity/registry.hpp>
#include <nlohmann/json.hpp>

namespace brk::loading::ut {
	struct C1
	{
		int x = 0, y = 1;
		static constexpr meta::FieldList<&C1::x, &C1::y> Fields{ "x", "y" };

		static constexpr StringView Name = "C1";
	};

	const nlohmann::json s_C1Json{ { "x", 1 }, { "y", 2 } };

	struct RAIIHelper
	{
		ecs::ComponentRegistry& m_Registry = ecs::ComponentRegistry::Init();
		entt::registry m_World;

		~RAIIHelper() { ecs::ComponentRegistry::Reset(); }
	};

	void LoadOneComponent()
	{
		RAIIHelper helper;
		const ecs::ComponentInfo& info = helper.m_Registry.Register<C1>();
		const entt::entity e = helper.m_World.create();
		info.m_LoadJson(s_C1Json, helper.m_World, e);
		const C1* comp = helper.m_World.try_get<const C1>(e);
		assert(comp);
		assert(comp->x == 1);
		assert(comp->y == 2);
	}

	void GetByName()
	{
		RAIIHelper helper;
		const ecs::ComponentInfo& info1 = helper.m_Registry.Register<C1>();
		const ecs::ComponentInfo& info2 = helper.m_Registry.GetInfo(C1::Name);
		assert(&info1 == &info2);
	}

	void ComponentInfoTests()
	{
		LoadOneComponent();
		GetByName();
	}
} // namespace brk::loading::ut
