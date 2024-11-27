#include <app/Entry.hpp>
#include "TestSystem.hpp"
#include <managers/ResourceManager.hpp>

void brk::RegisterGameSystems(brk::ecs::Manager& ecsManager)
{
	ecsManager.AddSystem<sandbox::TestSystem>(
		ResourceManager::GetInstance(),
		ecsManager.GetWorld());
}
void brk::RegisterGameComponents(brk::ecs::ComponentRegistry& registry) {}