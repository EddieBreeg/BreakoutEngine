#include <app/Entry.hpp>
#include "TestSystem.hpp"

void brk::RegisterGameSystems(brk::ecs::Manager& ecsManager)
{
	ecsManager.AddSystem<sandbox::TestSystem>();
}
void brk::RegisterGameComponents(brk::ecs::ComponentRegistry& registry) {}