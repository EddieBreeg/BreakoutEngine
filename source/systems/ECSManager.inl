#include <core/Assert.hpp>

template <class S, class... Args>
void brk::ecs::Manager::AddSystem(Args&&... args)
{
	const uint32 systemId = SystemId::Get<S>().m_Index;
	BRK_ASSERT(systemId >= m_Systems.size(), "System has already been registered!");
	m_Systems.emplace_back(SystemInstance::Create<S>(std::forward<Args>(args)...));
}