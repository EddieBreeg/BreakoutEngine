#include <app/Entry.hpp>
#include "TestSystem.hpp"
#include <managers/ECSManager.hpp>
#include <managers/ResourceManager.hpp>
#include <rendering/Material.hpp>
#include <rendering/Texture.hpp>
#include "Static.hpp"

void brk::RegisterGameSystems(brk::ecs::Manager& ecsManager)
{
	ecsManager.AddSystem<sandbox::TestSystem>(
		ResourceManager::GetInstance(),
		ecsManager.GetWorld());
}

void brk::RegisterGameComponents(brk::ecs::ComponentRegistry& registry) {}
void brk::RegisterGameResources(brk::ResourceManager& manager)
{
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