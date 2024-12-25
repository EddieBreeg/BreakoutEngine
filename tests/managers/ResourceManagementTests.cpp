#include <core/LogManager.hpp>
#include <core/Loaders.hpp>
#include <core/Resource.hpp>
#include <core/ResourceLoader.hpp>
#include <managers/ResourceManager.hpp>

#include <entt/entity/registry.hpp>
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
		ResourceTypeInfo& GetTypeInfo() const override;

		bool DoLoad()
		{
			m_State = Loaded;
			return true;
		}
	};
	struct Res2 : public Resource
	{
		Res2(ULID id)
			: Resource(id)
		{}
		static constexpr StringView Name = "Res2";
		ResourceTypeInfo& GetTypeInfo() const override;

		int32 m_Value = 0;
	};

	struct Res3 : public Resource
	{
		using Resource::Resource;
		Res3(uint32& count, ULID id)
			: Resource(id)
			, m_Count{ &++count }
		{}
		~Res3()
		{
			if (m_Count)
				--*m_Count;
		}

		static constexpr StringView Name = "res3";
		ResourceTypeInfo& GetTypeInfo() const override;

		uint32* m_Count = nullptr;
	};
} // namespace brk::resource_ref::ut

namespace brk {
	RES_INFO_IMPL_NO_ATTR(resource_ref::ut::Res1);
	RES_INFO_IMPL_NO_ATTR(resource_ref::ut::Res2);
	RES_INFO_IMPL_NO_ATTR(resource_ref::ut::Res3);
} // namespace brk

template <>
struct brk::JsonLoader<brk::resource_ref::ut::Res2>
{
	static bool Load(resource_ref::ut::Res2& out_res, const nlohmann::json& json)
	{
		return Visit("value", json, out_res.m_Value);
	}
	static void Save(const resource_ref::ut::Res2& res, nlohmann::json& out_json)
	{
		out_json["value"] = res.m_Value;
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
#if BRK_DEBUG
			: m_EntityWorld{ TrackerAllocator<entt::entity>{ m_AllocTracker } }
			, m_Manager{ ResourceManager::Init(m_EntityWorld) }
#else
			: m_Manager{ ResourceManager::Init(m_EntityWorld) }
#endif
		{
			ResourceLoader::Init();
			LogManager::GetInstance().m_Level = LogManager::Trace;
			m_Manager.RegisterResourceType<Res1>(Res1::Name);
			m_Manager.RegisterResourceType<Res2>(Res2::Name);
			m_Manager.RegisterResourceType<Res3>(Res3::Name);
		}
		~RAIIHelper()
		{
			m_Manager.Reset();
			ResourceTypeInfo::ResetFor<Res1>();
			ResourceTypeInfo::ResetFor<Res2>();
			ResourceTypeInfo::ResetFor<Res3>();
			ResourceLoader::Reset();
		}

#if BRK_DEBUG
		AllocTracker m_AllocTracker;
#endif
		ecs::EntityWorld m_EntityWorld;
		ResourceManager& m_Manager;
	};

	void ResourceRefTests()
	{
		{
			RAIIHelper helper;
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

			assert(ResourceLoader::GetInstance().GetQueueSize() == 1);

			ref->DoLoad();
			auto* rawPtr = ref.Get();
			ref.Reset();
			assert(rawPtr->GetLoadingState() == Resource::Unloading);
			{
				assert(ResourceLoader::GetInstance().GetQueueSize() == 2);
			}
			ResourceRef<Res1> ref2 = helper.m_Manager.GetRef<Res1>(s_ResId3);
			assert(!ref2);
		}
		{
			RAIIHelper helper;
			const nlohmann::json desc = {
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
		{
			RAIIHelper helper;
			uint32 count = 0;
			Res3& res = helper.m_Manager.AddResource<Res3>(count, s_ResId1);
			assert(count == 1);
			helper.m_Manager.DeleteResource(&res);
			assert(count == 0);
		}
		{
			RAIIHelper helper;
			uint32 count = 0;
			Res3& res = helper.m_Manager.AddResource<Res3>(count, s_ResId1);
			{
				ResourceRef<Res3> ref{ &res };
				assert(count == 1);
				helper.m_Manager.DeleteResource(&res);
				// resource shouldn't have been deleted, there's still a reference to it
				assert(count == 1);
			}
			assert(count == 0);
		}
	}
} // namespace brk::resource_ref::ut
