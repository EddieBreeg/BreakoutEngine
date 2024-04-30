#include "System.hpp"
#include <core/Assert.hpp>

brk::ecs::SystemInstance::SystemInstance(SystemInstance&& other)
	: m_Update{ other.m_Update }
	, m_Terminate{ other.m_Terminate }
{
	other.m_Update = nullptr;
	other.m_Terminate = nullptr;
}

void brk::ecs::SystemInstance::Update(entt::registry& world,
										   const TimeInfo& timeInfo)
{
	BRK_ASSERT(m_Update, "Tried to call Update on an invalid system instance");
	m_Update(m_SystemPtr, world, timeInfo);
}

brk::ecs::SystemInstance::~SystemInstance()
{
	if (m_Terminate)
		m_Terminate(m_SystemPtr);
}
