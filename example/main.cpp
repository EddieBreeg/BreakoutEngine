#include <app/Entry.hpp>
#include "TestSystem.hpp"
#include <managers/ECSManager.hpp>
#include <managers/ResourceManager.hpp>
#include <rendering/Material.hpp>
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
	manager.AddResource<rdr::Material>(settings, sandbox::s_MaterialId, "material1");
}