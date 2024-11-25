#include <core/FieldList.hpp>
#include <core/Loaders.hpp>
#include <ecs/ComponentRegistry.hpp>
#include <managers/SceneManager.hpp>
#include <nlohmann/json.hpp>
#include <managers/ECSManager.hpp>

namespace brk::scene_loading::ut {
	class RAIIHelper
	{
	public:
		RAIIHelper()
			: m_SceneManager{ SceneManager::Init() }
			, m_Registry{ ecs::ComponentRegistry::Init() }
			, m_ECSManager{ ecs::Manager::Init() }
			, m_World{ m_ECSManager.GetWorld() }
		{}

		~RAIIHelper()
		{
			m_Registry.Reset();
			m_SceneManager.Reset();
		}

		SceneManager& m_SceneManager;
		ecs::ComponentRegistry& m_Registry;
		ecs::Manager& m_ECSManager;
		entt::registry& m_World;
	};

	struct C1
	{
		int x = 0, y = 1;
		static constexpr meta::FieldList<&C1::x, &C1::y> Fields{ "x", "y" };

		static constexpr StringView Name = "C1";
	};

	constexpr StringView s_ObjectId1 = "01HXARTASQ9HX1SEJPF85A52VR";
	constexpr StringView s_SceneId1 = "01HXAM5KNMZDH2447595V5EH44";

	void Test00()
	{
#ifdef BRK_DEV
		RAIIHelper helper;
		helper.m_Registry.Register<C1>();
		const nlohmann::json sceneDesc{
			{
				{ "name", "TestScene" },
				{ "id", "01HXAM5KNMZDH2447595V5EH44" },
				{ "file", DATA_DIR "/test_scene0.brkscn" },
			},
		};
		helper.m_SceneManager.LoadSceneDescriptions(sceneDesc);
		helper.m_SceneManager.LoadScene(ULID::FromString(s_SceneId1));

		const ecs::GameObject* object =
			helper.m_SceneManager.GetObject(ULID::FromString(s_ObjectId1));
		assert(object);
		const C1* component = helper.m_World.try_get<const C1>(object->m_Entity);
		assert(component);
		assert(component->x == 1);
		assert(component->y == 2);
#endif
	}

	void Tests()
	{
		Test00();
	}
} // namespace brk::scene_loading::ut
