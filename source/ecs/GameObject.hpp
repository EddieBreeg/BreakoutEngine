#pragma once

#include <PCH.hpp>
#include <core/ULID.hpp>
#include "ComponentRegistry.hpp"
#include <vector>

namespace brk::ecs
{
	struct GameObject
	{
		ULID m_Id;
		entt::entity m_Entity;
		std::vector<const ComponentInfo*> m_Components;
	};
	
} // namespace brk::ecs
