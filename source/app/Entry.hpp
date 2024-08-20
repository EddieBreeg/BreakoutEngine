#pragma once

#include "App.hpp"

namespace brk::ecs
{
	class Manager;
} // namespace brk::ecs


namespace brk
{
	extern void RegisterGameSystems(ecs::Manager& ecsManager);
} // namespace brk
