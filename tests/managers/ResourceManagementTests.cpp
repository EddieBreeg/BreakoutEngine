#include <managers/ResourceManager.hpp>
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <entt/entity/registry.hpp>
#include <systems/ResourceLoadingComponents.hpp>
#include <cassert>

namespace brk::resource_ref::ut {
	struct Res1 : public Resource
	{
		using Resource::Resource;
		static constexpr StringView Name = "Res1";
	};

	constexpr ULID s_ResId1 = ULID::FromString("01HXARTASQ9HX1SEJPF85A52VR");
	constexpr ULID s_ResId2 = ULID::FromString("01HXAM5KNMZDH2447595V5EH44");

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
		{
			RAIIHelper helper;
			helper.m_Manager.RegisterResourceType<Res1>();
			const nlohmann::json desc{
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
			helper.m_Manager.PreloadResources(desc);
			ResourceRef<const Res1> ref = helper.m_Manager.GetRef<const Res1>(s_ResId1);
			assert(ref.IsValid());
			assert(ref->GetLoadingState() == Resource::Loading);

			auto view = helper.m_EntityWorld.view<ResourceLoadRequestComponent>();
			int reqCount = 0;
			for (const entt::entity req : view)
				++reqCount;
			assert(reqCount == 1);
		}
	}
} // namespace brk::resource_ref::ut
