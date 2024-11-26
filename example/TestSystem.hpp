#pragma once

#include <PCH.hpp>
#include <ecs/World.hpp>
#include <rendering/Buffer.hpp>

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

		~TestSystem() = default;

	private:
		rdr::Buffer m_Vbo, m_Ibo, m_ParamBuff;
	};
} // namespace brk::sandbox
