#include "ECSManager.hpp"
#include <systems/WindowSystem.hpp>

#include <SDL2/SDL.h>

brk::ecs::Manager::Manager()
{
	InitEngineSystems();
}

void brk::ecs::Manager::InitEngineSystems()
{
	{
		WindowSystemSettings settings;
#ifdef BRK_DEV
		settings.m_Flags |= SDL_WINDOW_RESIZABLE;
#endif
		AddSystem<WindowSystem>(settings);
	}
}

void brk::ecs::Manager::Update(const TimeInfo& timeInfo)
{
	for (SystemInstance& system : m_Systems)
		system.Update(m_EntityWorld, timeInfo);
}
