#pragma once

#include <PCH.hpp>
#include <ecs/World.hpp>
#include <rendering/Buffer.hpp>
#include <rendering/Shaders.hpp>

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
		rdr::Buffer m_Vbo, m_Ibo, m_ParamBuff;
		rdr::VertexShader m_VertexShader;
		rdr::FragmentShader m_FragmentShader;
	};
} // namespace brk::sandbox
