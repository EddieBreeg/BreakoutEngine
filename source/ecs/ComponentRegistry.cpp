#include "ComponentRegistry.hpp"
#ifdef BRK_DEV
#include <core/StringViewFormatter.hpp>
#endif

const brk::ecs::ComponentInfo& brk::ecs::ComponentRegistry::GetInfo(
	const StringView name) const
{
	const uint32 h = Hash<StringView>{}(name);
	const auto it = m_TypeMap.find(h);
	BRK_ASSERT(it != m_TypeMap.end(), "No component info found for name '{}'", name);
	return it->second;
}