#include "Entry.hpp"

#include <ecs/ComponentRegistry.hpp>

#include <core/Loaders.hpp>

#include <managers/ResourceManager.hpp>

#include <entt/entity/registry.hpp>

#include <editor/ui/ComponentWidgets.hpp>

#include <rendering/Material.hpp>

#include <systems/TransformComponent.hpp>
#include "App.hpp"

void brk::App::RegisterComponents(const EntryPoint& entryPoint)
{
	auto& registry = ecs::ComponentRegistry::GetInstance();

	registry.Register<TransformComponent>(&editor::TransformWidget);
	if (entryPoint.RegisterGameComponents)
		entryPoint.RegisterGameComponents(registry);
}

void brk::App::RegisterResources(const EntryPoint& entryPoint)
{
	auto& manager = ResourceManager::GetInstance();
	manager.RegisterResourceType<rdr::Material>();
	if (entryPoint.RegisterGameResources)
		entryPoint.RegisterGameResources(manager);
}