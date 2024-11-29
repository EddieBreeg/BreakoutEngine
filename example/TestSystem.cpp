#include "TestSystem.hpp"
#include <core/TimeInfo.hpp>
#include <managers/ResourceManager.hpp>
#include <math/Constants.hpp>
#include <rendering/Renderer.hpp>
#include <rendering/Shaders.hpp>
#include <rendering/Vertex.hpp>
#include <rendering/Texture.hpp>
#include <systems/VisualComponents.hpp>
#include "Static.hpp"

namespace {
	static constexpr uint32 s_Indices[] = { 0, 1, 2 };
	static constexpr brk::rdr::Vertex3d s_Vertices[] = {
		{ { -.5f, -.5f, 0 }, { 0, 0, 1 }, { 0, 0 } },
		{ { .5f, -.5f, 0 }, { 0, 0, 1 }, { 1, 0 } },
		{ { 0, .5f, 0 }, { 0, 0, 1 }, { 1, 1 } },
	};

	struct FragmentParams
	{
		float4 m_DiffuseColor;
	};
} // namespace

brk::sandbox::TestSystem::TestSystem(
	ResourceManager& resManager,
	entt::registry& entityWorld)
	: m_Mesh{ resManager.AddResource<rdr::Mesh>(
		  s_Vertices,
		  s_Indices,
		  ULID::Generate(),
		  "mesh1") }
	, m_MatInstance{ resManager.AddResource<rdr::MaterialInstance>(
		  resManager.GetRef<rdr::Material>(s_MaterialId),
		  ULID::Generate(),
		  "mat_instance") }
	, m_Texture{ resManager.GetRef<rdr::Texture2d>(s_TextureId) }
{
	const auto e = entityWorld.create();
	entityWorld.emplace<VisualComponent>(e);
	entityWorld.emplace<MeshComponent>(
		e,
		ResourceRef<rdr::MaterialInstance>{ &m_MatInstance },
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
	m_MatInstance.SetParameters(params);
}
