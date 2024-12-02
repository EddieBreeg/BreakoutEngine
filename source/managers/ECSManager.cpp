#include "ECSManager.hpp"
#include <systems/WindowSystem.hpp>

std::unique_ptr<brk::ecs::Manager> brk::ecs::Manager::s_Instance;

brk::ecs::Manager::Manager() = default;

void brk::ecs::Manager::Update(const TimeInfo& timeInfo)
{
	for (SystemInstance& system : m_Systems)
		system.Update(m_EntityWorld, timeInfo);
}

void brk::ecs::Manager::Clear()
{
	m_Systems.clear();
	m_EntityWorld.clear();
}

brk::ecs::Manager::~Manager()
{
	Clear();
}
