#include "Entry.hpp"

#include <ecs/ComponentRegistry.hpp>

#include <core/Loaders.hpp>

#include <managers/ResourceManager.hpp>

#include <editor/ui/ComponentWidgets.hpp>

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
	RegisterGameResources(manager);
}