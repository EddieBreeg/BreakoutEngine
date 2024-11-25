#include <core/Assert.hpp>

template <class S, class... Args>
void brk::ecs::Manager::AddSystem(Args&&... args)
{
	BRK_ASSERT(s_SysIndex<S> >= m_Systems.size(), "System has already been registered!");
	s_SysIndex<S> = m_Systems.size();
	m_Systems.emplace_back(SystemInstance::Create<S>(std::forward<Args>(args)...));
}