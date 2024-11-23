#include "Entry.hpp"

#include <ecs/ComponentRegistry.hpp>

#include <core/Loaders.hpp>

#include <editor/ui/ComponentWidgets.hpp>

#include <systems/TransformComponent.hpp>

void brk::App::RegisterComponents()
{
	auto& registry = ecs::ComponentRegistry::GetInstance();

	registry.Register<TransformComponent>(&editor::TransformWidget);

	RegisterGameComponents(registry);
}