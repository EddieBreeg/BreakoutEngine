#pragma once

#include <PCH.hpp>
#include <core/ULID.hpp>
#include "ComponentRegistry.hpp"
#include <vector>
#include <string>

namespace brk::ecs {
	struct GameObject
	{
		ULID m_Id;
		std::string m_Name;
		entt::entity m_Entity;
		std::vector<const ComponentInfo*> m_Components;
	};
} // namespace brk::ecs
