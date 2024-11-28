#pragma once

#include <PCH.hpp>
#include <ecs/World.hpp>

namespace brk {
	class ResourceManager;
	class TimeInfo;
} // namespace brk

namespace brk::rdr {
	class Material;
	class MaterialInstance;
	class Mesh;
	class Texture2d;
} // namespace brk::rdr

namespace brk::sandbox {
	class TestSystem
	{
	public:
		using World = ecs::WorldView<>;

		TestSystem(ResourceManager& resourceManager, entt::registry& entityWorld);
		void Update(World&, const TimeInfo&);

		~TestSystem();

	private:
		rdr::Mesh& m_Mesh;
		rdr::MaterialInstance& m_MatInstance;
		rdr::Texture2d& m_Texture;
	};
} // namespace brk::sandbox
