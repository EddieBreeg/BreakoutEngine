#pragma once

#include "App.hpp"

namespace brk::ecs
{
	class ComponentRegistry;
	class Manager;
} // namespace brk::ecs


namespace brk
{
	extern void RegisterGameSystems(ecs::Manager& ecsManager);
	extern void RegisterGameComponents(ecs::ComponentRegistry& registry);
} // namespace brk
