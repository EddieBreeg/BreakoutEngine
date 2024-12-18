#include <core/FieldList.hpp>
#include <core/Loaders.hpp>
#include <core/Resource.hpp>
#include <core/ResourceLoader.hpp>
#include <ecs/ComponentRegistry.hpp>
#include <managers/SceneManager.hpp>
#include <nlohmann/json.hpp>
#include <managers/ECSManager.hpp>
#include <managers/ResourceManager.hpp>

namespace {
	constexpr brk::ULID s_ResId1 = brk::ULID::FromString("01JDWW0RSFRTBPNEK2X0TEETNB");
}

namespace brk::scene_loading::ut {
	struct Res : public Resource
	{
		using Resource::Resource;
		const ResourceTypeInfo& GetTypeInfo() const noexcept override;
	};

	class RAIIHelper
	{
	public:
		RAIIHelper()
			: m_ResLoader{ ResourceLoader::Init() }
			, m_SceneManager{ SceneManager::Init() }
			, m_Registry{ ecs::ComponentRegistry::Init() }
			, m_ECSManager{ ecs::Manager::Init() }
			, m_World{ m_ECSManager.GetWorld() }
			, m_ResManager{ ResourceManager::Init(m_World) }
		{
			m_ResManager.RegisterResourceType<Res>("res");
			m_Resource = &m_ResManager.AddResource<Res>(s_ResId1);
		}

		~RAIIHelper()
		{
			m_ECSManager.Clear();
			m_Registry.Reset();
			m_SceneManager.Reset();
			m_ResManager.Reset();
			ResourceLoader::Reset();
		}

		ResourceLoader& m_ResLoader;
		SceneManager& m_SceneManager;
		ecs::ComponentRegistry& m_Registry;
		ecs::Manager& m_ECSManager;
		entt::registry& m_World;
		ResourceManager& m_ResManager;
		Res* m_Resource;
	};

	struct C1
	{
		int x = 0, y = 1;
		static constexpr meta::FieldList<&C1::x, &C1::y> Fields{ "x", "y" };

		static inline const ecs::ComponentInfo Info =
			ecs::ComponentInfo::Create<C1>("C1");
	};

	struct C2
	{
		ResourceRef<Res> m_Ref;
		static constexpr meta::FieldList<&C2::m_Ref> Fields{ "resId" };
		static inline const ecs::ComponentInfo Info =
			ecs::ComponentInfo::Create<C2>("C2");
	};

	constexpr ULID s_ObjectId1 = ULID::FromString("01HXARTASQ9HX1SEJPF85A52VR");
	constexpr ULID s_ObjectId2 = ULID::FromString("01JDWW4FT9SA2XT2EVV936SN0A");
	constexpr ULID s_ObjectId3 = ULID::FromString("01JDWXT1HXZAG7T3H9AD7MAVN1");
	constexpr StringView s_SceneId1 = "01HXAM5KNMZDH2447595V5EH44";

	void Test00()
	{
#ifdef BRK_DEV
		RAIIHelper helper;
		helper.m_Registry.Register<C1>();
		helper.m_Registry.Register<C2>();
		const nlohmann::json sceneDesc{
			{
				{ "name", "TestScene" },
				{ "id", s_SceneId1 },
				{ "file", DATA_DIR "/test_scene0.brkscn" },
			},
		};
		helper.m_SceneManager.LoadSceneDescriptions(sceneDesc);
		helper.m_SceneManager.LoadScene(ULID::FromString(s_SceneId1));
		helper.m_ResLoader.ProcessBatch();
		helper.m_ResLoader.Wait();

		{
			const ecs::GameObject* object = helper.m_SceneManager.GetObject(s_ObjectId1);
			assert(object);
			const C1* component = helper.m_World.try_get<const C1>(object->m_Entity);
			assert(component);
			assert(component->x == 1);
			assert(component->y == 2);
		}
		{
			const ecs::GameObject* object = helper.m_SceneManager.GetObject(s_ObjectId2);
			assert(object);
			const C2* component = helper.m_World.try_get<const C2>(object->m_Entity);
			assert(component);
			assert(component->m_Ref);
			assert(component->m_Ref->GetId() == s_ResId1);
		}
		{
			const ecs::GameObject* object = helper.m_SceneManager.GetObject(s_ObjectId3);
			assert(object);
			const C2* component = helper.m_World.try_get<const C2>(object->m_Entity);

			// loading the component should have failed, as the resource id was invalid
			assert(!component);
		}
#endif
	}

	void Tests()
	{
		Test00();
	}
} // namespace brk::scene_loading::ut

namespace brk {
	RES_INFO_IMPL_NO_ATTR(scene_loading::ut::Res);
}