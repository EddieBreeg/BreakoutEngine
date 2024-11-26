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
	, m_Material{ float4{ 1, 0, 0, 0 }, {}, s_ShaderSource }
{
	auto& pipelineState = rdr::Renderer::s_Instance.GetData()->m_CurrentPipelineState;
	pipelineState.m_VertexBuffer = m_Vbo.GetHandle(),
	pipelineState.m_IndexBuffer = m_Ibo.GetHandle(),

	rdr::Renderer::s_Instance.SetMaterial(m_Material);
}

brk::sandbox::TestSystem::~TestSystem() = default;

void brk::sandbox::TestSystem::Update(World&, const TimeInfo&) {}
