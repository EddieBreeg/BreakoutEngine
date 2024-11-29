#include <managers/ResourceManager.hpp>
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <entt/entity/registry.hpp>
#include <systems/ResourceLoadingSystem.hpp>
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

		// IMPORTANT: if the refcount isn't 0, the resource can't be deleted
		void ResetRefCount() { m_RefCount = 0; }

		static constexpr StringView Name = "Res1";
		bool DoLoad()
		{
			m_LoadingState = Loaded;
			return true;
		}
	};
	struct Res2 : public Resource
	{
		Res2(ULID id)
			: Resource(id)
		{}
		static constexpr StringView Name = "Res2";
		int32 m_Value = 0;
	};
} // namespace brk::resource_ref::ut
template <>
struct brk::JsonLoader<brk::resource_ref::ut::Res2>
{
	static bool Load(resource_ref::ut::Res2& out_res, const nlohmann::json& json)
	{
		return Visit("value", json, out_res.m_Value);
	}
};

namespace brk::resource_ref::ut {
	constexpr ULID s_ResId1 = ULID::FromString("01HXARTASQ9HX1SEJPF85A52VR");
	constexpr ULID s_ResId2 = ULID::FromString("01HXAM5KNMZDH2447595V5EH44");
	constexpr ULID s_ResId3 = ULID::FromString("01JDS4NMGSWAH6MH5JZ7190Q9R");

	constexpr const char* s_ResName1 = "resource1";
	constexpr const char* s_ResName2 = "resource2";
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
				{
					// missing type key, loading should fail
					{ "id", s_ResId3 },
					{ "name", "r3" },
				},
			};
			helper.m_Manager.CreateResources(desc);
			ResourceRef<Res1> ref = helper.m_Manager.GetRef<Res1>(s_ResId1);
			assert(ref);
			assert(ref->GetLoadingState() == Resource::Loading);

			assert(ResourceLoadingRequests::s_Instance.m_LoadRequests.size() == 1);

			ref->DoLoad();
			auto* rawPtr = ref.Get();
			ref.Reset();
			assert(rawPtr->GetLoadingState() == Resource::Unloading);
			{
				assert(ResourceLoadingRequests::s_Instance.m_UnloadRequests.size() == 1);
			}
			ResourceRef<Res1> ref2 = helper.m_Manager.GetRef<Res1>(s_ResId3);
			assert(!ref2);
		}
		{
			RAIIHelper helper;
			helper.m_Manager.RegisterResourceType<Res2>();
			const std::vector<nlohmann::json> desc{
				{
					// should fail, we didn't specify value key
					{ "type", Res2::Name },
					{ "name", s_ResName1 },
					{ "id", s_ResId1 },
				},
				{
					{ "type", Res2::Name },
					{ "name", s_ResName2 },
					{ "id", s_ResId2 },
					{ "value", 1 },
				},
			};
			helper.m_Manager.CreateResources(desc);
			{
				ResourceRef<Res2> ref = helper.m_Manager.GetRef<Res2>(s_ResId1);
				assert(!ref);
			}
			{
				ResourceRef<Res2> ref = helper.m_Manager.GetRef<Res2>(s_ResId2);
				assert(ref);
				assert(ref->m_Value == 1);
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
			res.ResetRefCount();
		}
	}
} // namespace brk::resource_ref::ut
