#include "TestSystem.hpp"
#include <rendering/Renderer.hpp>
#include <rendering/dx11/Renderer.hpp>
#include <rendering/Shaders.hpp>
#include <rendering/Vertex.hpp>
#include "d3d11.h"

namespace {
	static constexpr uint32 s_Indices[] = { 0, 1, 2 };
	static constexpr brk::rdr::Vertex3d s_Vertices[] = {
		{ { -.5f, -.5f, 0 }, { 0, 0, 1 }, { 0, 0 } },
		{ { .5f, -.5f, 0 }, { 0, 0, 1 }, { 1, 0 } },
		{ { 0, .5f, 0 }, { 0, 0, 1 }, { 1, 1 } },
	};

	constexpr const char* s_ShaderSource = R"hlsl(
cbuffer Params: register(b0)
{
	float4 DiffuseColor;
}

struct Vertex
{
	float3 Position: POSITION;
	float3 Normal: NORMAL;
	float2 Uv: TEXCOORD;
};

float4 vs_main(Vertex v): SV_POSITION
{
	return float4(v.Position, 1);
}

float4 fs_main(float4 fragPos: SV_POSITION): SV_TARGET
{
	return DiffuseColor;
}
)hlsl";

	struct FragmentParams
	{
		float4 m_DiffuseColor;
	};
} // namespace

brk::sandbox::TestSystem::TestSystem()
	: m_Vbo{ rdr::Buffer::VertexBuffer, s_Vertices }
	, m_Ibo{ rdr::Buffer::IndexBuffer, s_Indices }
	, m_ParamBuff{ rdr::Buffer::ParamBuffer,
				   sizeof(FragmentParams),
				   nullptr,
				   EnumFlags{ rdr::EBufferOptions::Dynamic } }
	, m_VertexShader{ s_ShaderSource }
	, m_FragmentShader{ s_ShaderSource }
{
	rdr::Renderer::s_Instance.GetData()->m_CurrentPipelineState = {
		m_Vbo.GetHandle(),
		m_Ibo.GetHandle(),
		m_ParamBuff.GetHandle(),
		m_VertexShader.GetHandle(),
		m_FragmentShader.GetHandle(),
	};
}

brk::sandbox::TestSystem::~TestSystem() = default;

void brk::sandbox::TestSystem::Update(World&, const TimeInfo&)
{
	const FragmentParams params = { { 1.0f, 0.0f, 0.0f, 1.0f } };
	m_ParamBuff.SetData(params);
}
