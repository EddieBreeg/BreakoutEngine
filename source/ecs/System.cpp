#include "System.hpp"
#include <core/Assert.hpp>

brk::ecs::SystemInstance::SystemInstance(SystemInstance&& other)
	: m_SystemPtr{ other.m_SystemPtr }
	, m_Update{ other.m_Update }
	, m_Terminate{ other.m_Terminate }
{
	other.m_SystemPtr = nullptr;
	other.m_Update = nullptr;
	other.m_Terminate = nullptr;
}

void brk::ecs::SystemInstance::Update(EntityWorld& world, const TimeInfo& timeInfo)
{
	BRK_ASSERT(m_Update, "Tried to call Update on an invalid system instance");
	m_Update(m_SystemPtr, world, timeInfo);
}

brk::ecs::SystemInstance::~SystemInstance()
{
	if (m_Terminate)
		m_Terminate(m_SystemPtr);
}
