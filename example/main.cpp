#include <app/Entry.hpp>
#include "TestSystem.hpp"
#include <managers/ECSManager.hpp>
#include <managers/ResourceManager.hpp>
#include <rendering/Material.hpp>
#include <rendering/Mesh.hpp>
#include <rendering/Texture.hpp>
#include "Static.hpp"

namespace {
	static constexpr brk::rdr::Vertex3d s_PlaneVertices[] = {
		{ { -.5f, -.5f, 0 }, { 0, 0, 1 }, { 0, 0 } },
		{ { .5f, -.5f, 0 }, { 0, 0, 1 }, { 1, 0 } },
		{ { .5f, .5f, 0 }, { 0, 0, 1 }, { 1, 1 } },
		{ { -.5f, .5f, 0 }, { 0, 0, 1 }, { 0, 1 } },
	};
	static constexpr uint32 s_PlaneIndices[] = { 0, 1, 2, 2, 3, 0 };

	void RegisterGameSystems(brk::ecs::Manager& ecsManager)
	{
		using namespace brk;

		ecsManager.AddSystem<sandbox::TestSystem>(
			ResourceManager::GetInstance(),
			ecsManager.GetWorld());
	}

	void RegisterGameResources(brk::ResourceManager& manager)
	{
		using namespace brk;

		constexpr rdr::MaterialSettings settings{
			{},
			sandbox::s_FragShaderSource,
			rdr::MaterialSettings::DynamicBufferParam,
		};
		constexpr rdr::Texture2dSettings texSettings = {
			rdr::EPixelFormat::RGBA8,
		};

		auto& baseMaterial = manager.AddResource<rdr::Material>(
			settings,
			sandbox::s_MaterialId,
			"material1");
		manager.AddResource<rdr::Texture2d>(
			texSettings,
			sandbox::s_TextureId,
			"texture1",
			"Data/Textures/breakout.png");
		manager.AddResource<rdr::Mesh>(
			s_PlaneVertices,
			s_PlaneIndices,
			brk::sandbox::s_MeshId,
			"plane");
		auto& matInstance = manager.AddResource<rdr::MaterialInstance>(
			ResourceRef<rdr::Material>{ &baseMaterial },
			float4{ 1, 1, 1, 1 },
			sandbox::s_MaterialInstanceId,
			"mat_instance");
		matInstance.SetTextureId(0, sandbox::s_TextureId);
	}
} // namespace

brk::EntryPoint brk::CreateEntryPoint()
{
	return { &RegisterGameSystems, nullptr, &RegisterGameResources };
}