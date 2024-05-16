#include <managers/ResourceManager.hpp>
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <cassert>

namespace brk::resource_ref::ut {
	struct Res1 : public Resource
	{
		static int n;
		Res1() { ++n; }
		~Res1() { --n; }

		static constexpr StringView Name = "Res1";
	};
	int Res1::n = 0;

	constexpr ULID s_ResId1 = ULID::FromString("01HXARTASQ9HX1SEJPF85A52VR");

	class RAIIHelper
	{
	public:
		RAIIHelper()
			: m_Manager{ ResourceManager::Init() }
		{
			LogManager::GetInstance().m_Level = LogManager::Trace;
		}
		~RAIIHelper() { m_Manager.Reset(); }

		ResourceManager& m_Manager;
	};

	void ResourceRefTests()
	{
		{
			RAIIHelper helper;
			helper.m_Manager.RegisterResourceType<Res1>();
			const nlohmann::json desc{ { { "type", Res1::Name },
										 { "id", s_ResId1 },
										 { "name", "r1" },
										 { "file", "r1.res" } } };
			helper.m_Manager.PreloadResources(desc);
			assert(Res1::n == 1);
			ResourceRef<const Res1> ref = helper.m_Manager.GetRef<const Res1>(s_ResId1);
			assert(ref.IsValid());
		}
		assert(Res1::n == 0);
		{
			RAIIHelper helper;
			helper.m_Manager.RegisterResourceType<Res1>();
			const nlohmann::json desc{
				{
					{ "type", Res1::Name },
					{ "name", "test" },
				},
			};
			helper.m_Manager.PreloadResources(desc);
			assert(Res1::n == 0);
		}
	}
} // namespace brk::resource_ref::ut
