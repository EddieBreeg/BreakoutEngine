#include "Entry.hpp"

#include <ecs/ComponentRegistry.hpp>

#include <core/Loaders.hpp>

#include <managers/ResourceManager.hpp>

#include <entt/entity/registry.hpp>

#include <editor/ui/ComponentWidgets.hpp>

#include <rendering/Material.hpp>

#include <systems/TransformComponent.hpp>
#include "App.hpp"

void brk::App::RegisterComponents()
{
	auto& registry = ecs::ComponentRegistry::GetInstance();

	registry.Register<TransformComponent>(&editor::TransformWidget);
	RegisterGameComponents(registry);
}

void brk::App::RegisterResources()
{
	auto& manager = ResourceManager::GetInstance();
	manager.RegisterResourceType<rdr::Material>();
	RegisterGameResources(manager);
}