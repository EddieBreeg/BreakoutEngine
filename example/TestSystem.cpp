#include "TestSystem.hpp"
#include <core/TimeInfo.hpp>
#include <managers/ResourceManager.hpp>
#include <math/Constants.hpp>
#include <rendering/Renderer.hpp>
#include <rendering/Shaders.hpp>
#include <rendering/Vertex.hpp>
#include <systems/VisualComponents.hpp>

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

	constexpr brk::rdr::MaterialSettings s_MatSettings = {
		{},
		s_ShaderSource,
		brk::rdr::MaterialSettings::DynamicBufferParam,
	};
} // namespace

brk::sandbox::TestSystem::TestSystem(
	ResourceManager& resManager,
	entt::registry& entityWorld)
	: m_Mesh{ resManager.AddResource<rdr::Mesh>(s_Vertices, s_Indices) }
	, m_Material{
		resManager.AddResource<rdr::Material>(float4{ 1, 0, 0, 0 }, s_MatSettings)
	}
{
	const auto e = entityWorld.create();
	entityWorld.emplace<VisualComponent>(e);
	entityWorld.emplace<MeshComponent>(
		e,
		ResourceRef<rdr::Material>{ &m_Material },
		ResourceRef<rdr::Mesh>{ &m_Mesh });
}

brk::sandbox::TestSystem::~TestSystem() = default;

void brk::sandbox::TestSystem::Update(World&, const TimeInfo& time)
{
	float t = time.GetElapsed().count() * 3;
	const FragmentParams params{ {
		.5f * std::sinf(t * 3.0f) + .5f,
		.5f * std::sinf(t * 3.0f + .33f * math::Pi) + .5f,
		.5f * std::sinf(t * 3.0f + .6667f * math::Pi) + .5f,
		1,
	} };
	m_Material.SetParameters(params);
}
