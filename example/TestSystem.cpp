#include "TestSystem.hpp"
#include <core/TimeInfo.hpp>
#include <entt/entity/registry.hpp>
#include <managers/ResourceManager.hpp>
#include <math/Constants.hpp>
#include <rendering/Material.hpp>
#include <rendering/Renderer.hpp>
#include <rendering/Shaders.hpp>
#include <rendering/Vertex.hpp>
#include <rendering/Texture.hpp>
#include <systems/VisualComponents.hpp>

namespace {
	struct FragmentParams
	{
		float4 m_DiffuseColor;
	};
} // namespace

brk::sandbox::TestSystem::TestSystem(
	ResourceManager& resManager,
	ecs::EntityWorld& entityWorld)
{}

brk::sandbox::TestSystem::~TestSystem() = default;

void brk::sandbox::TestSystem::Update(World& world, const TimeInfo& time)
{
	float t = time.GetElapsed().count() * 3;
	const FragmentParams params{ {
		.5f * std::sinf(t * 3.0f) + .5f,
		.5f * std::sinf(t * 3.0f + .33f * math::Pi) + .5f,
		.5f * std::sinf(t * 3.0f + .6667f * math::Pi) + .5f,
		1,
	} };
	auto view = world.Query<ecs::query::Include<MeshComponent>>();
	for (auto entity : view)
	{
		auto& mesh = view.Get<MeshComponent>(entity);
		if (!mesh)
			continue;
		mesh.m_MaterialRef->SetParameters(params);
	}
}
