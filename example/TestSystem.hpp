#pragma once

#include <PCH.hpp>
#include <ecs/World.hpp>
#include <rendering/Material.hpp>
#include <rendering/Mesh.hpp>

namespace brk {
	class TimeInfo;
}

namespace brk::sandbox {
	class TestSystem
	{
	public:
		using World = ecs::WorldView<>;

		TestSystem();
		void Update(World&, const TimeInfo&);

		~TestSystem();

	private:
		rdr::Mesh m_Mesh;
		rdr::Material m_Material;
	};
} // namespace brk::sandbox
