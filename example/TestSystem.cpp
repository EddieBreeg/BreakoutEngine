#include "d3d11.h"
#include "TestSystem.hpp"
#include <rendering/dx11/Shaders.hpp>
#include <rendering/Shaders.hpp>
#include <rendering/Vertex.hpp>

namespace {
	static constexpr uint32 s_Indices[] = { 0, 1, 2 };
	static constexpr brk::rdr::Vertex3d s_Vertices[] = {
		{ { -.5f, -.5f, -1.0f }, { 0, 0, 1 }, { 0, 0 } },
		{ { .5f, -.5f, -1.0f }, { 0, 0, 1 }, { 1, 0 } },
		{ { 0, .5f, -1.0f }, { 0, 0, 1 }, { 1, 1 } },
	};

	struct FragmentParams
	{
		float4 m_DiffuseColor;
	};
} // namespace

brk::sandbox::TestSystem::TestSystem()
	: m_Vbo{ rdr::Buffer::VertexBuffer, s_Vertices }
	, m_Ibo{ rdr::Buffer::IndexBuffer, s_Indices }
	, m_ParamBuff{
		rdr::Buffer::ParamBuffer,
		sizeof(FragmentParams),
		nullptr,
		EnumFlags{ rdr::EBufferOptions::Dynamic },
	}
{}

brk::sandbox::TestSystem::~TestSystem() = default;

void brk::sandbox::TestSystem::Update(World&, const TimeInfo&)
{
	const FragmentParams params = { { 1.0f, 0.0f, 0.0f, 1.0f } };
	m_ParamBuff.SetData(params);
}
