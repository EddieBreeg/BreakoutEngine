#include <app/Entry.hpp>
#include "TestSystem.hpp"
#include <managers/ECSManager.hpp>
#include <managers/ResourceManager.hpp>
#include <rendering/Material.hpp>
#include <rendering/Texture.hpp>
#include "Static.hpp"

namespace {
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

		manager.AddResource<rdr::Material>(settings, sandbox::s_MaterialId, "material1");
		manager.AddResource<rdr::Texture2d>(
			texSettings,
			sandbox::s_TextureId,
			"texture1",
			"Data/Textures/breakout.png");
	}
} // namespace

brk::EntryPoint brk::CreateEntryPoint()
{
	return { &RegisterGameSystems, nullptr, &RegisterGameResources };
}