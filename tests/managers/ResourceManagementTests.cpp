#include <managers/ResourceManager.hpp>
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <entt/entity/registry.hpp>
#include <systems/ResourceLoadingComponents.hpp>
#include <cassert>

namespace brk::resource_ref::ut {
	struct Res1 : public Resource
	{
		Res1(ULID id)
			: Resource(id)
		{}
		Res1(ULID id, std::string name, std::string file)
			: Resource(id, std::move(name), std::move(file))
		{
			m_RefCount = 1;
		}

		static constexpr StringView Name = "Res1";
		bool DoLoad()
		{
			m_LoadingState = Loaded;
			return true;
		}
	};

	constexpr ULID s_ResId1 = ULID::FromString("01HXARTASQ9HX1SEJPF85A52VR");
	constexpr ULID s_ResId2 = ULID::FromString("01HXAM5KNMZDH2447595V5EH44");

	constexpr const char* s_ResName1 = "resource1";
	constexpr const char* s_ResPath1 = "resource1.res";

	class RAIIHelper
	{
	public:
		RAIIHelper()
			: m_Manager{ ResourceManager::Init(m_EntityWorld) }
		{
			LogManager::GetInstance().m_Level = LogManager::Trace;
		}
		~RAIIHelper() { m_Manager.Reset(); }

		entt::registry m_EntityWorld;
		ResourceManager& m_Manager;
	};

	void ResourceRefTests()
	{
#ifdef BRK_DEV
		{
			RAIIHelper helper;
			helper.m_Manager.RegisterResourceType<Res1>();
			const std::vector<nlohmann::json> desc{
				{
					{ "type", Res1::Name },
					{ "id", s_ResId1 },
					{ "name", "r1" },
					{ "file", "r1.res" },
				},
				{
					{ "type", Res1::Name },
					{ "id", s_ResId2 },
					{ "name", "r2" },
					{ "file", "r2.res" },
				},
			};
			helper.m_Manager.CreateResources(desc);
			ResourceRef<Res1> ref = helper.m_Manager.GetRef<Res1>(s_ResId1);
			assert(ref);
			assert(ref->GetLoadingState() == Resource::Loading);

			{
				auto view = helper.m_EntityWorld.view<ResourceLoadRequestComponent>();
				int reqCount = 0;
				for (const entt::entity req : view)
					++reqCount;
				assert(reqCount == 1);
			}
			ref->DoLoad();
			auto* rawPtr = ref.Get();
			ref.Reset();
			assert(rawPtr->GetLoadingState() == Resource::Unloading);
			{
				auto view = helper.m_EntityWorld.view<ResourceUnloadRequestComponent>();
				int reqCount = 0;
				for (const entt::entity req : view)
					++reqCount;
				assert(reqCount == 1);
			}
		}
#endif
		{
			RAIIHelper helper;
			Res1& res =
				helper.m_Manager.AddResource<Res1>(s_ResId1, s_ResName1, s_ResPath1);
			assert(res.GetId() == s_ResId1);
			assert(res.GetName() == s_ResName1);
			assert(res.GetFile() == s_ResPath1);
			assert(RetainTraits<Resource>::GetCount(&res) == 1);
		}
	}
} // namespace brk::resource_ref::ut
