#pragma once

#include <PCH.hpp>
#include <ecs/World.hpp>
#include <rendering/Buffer.hpp>
#include <rendering/Material.hpp>

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
		rdr::Buffer m_Vbo, m_Ibo;
		rdr::Material m_Material;
	};
} // namespace brk::sandbox
