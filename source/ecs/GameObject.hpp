#pragma once

#include <PCH.hpp>
#include <core/ULID.hpp>
#include <entt/entity/fwd.hpp>
#include <vector>
#include <string>

namespace brk::ecs {
	struct ComponentInfo;

	/**
	 * Represents a game object with a scene
	 */
	struct GameObject
	{
		ULID m_Id;
		std::string m_Name;
		entt::entity m_Entity;
		struct Component
		{
			Component(const ComponentInfo& info, void* widget)
				: m_Info{ &info }
				, m_Widget{ widget }
			{}
			const ComponentInfo* m_Info = nullptr;
			void* m_Widget = nullptr;
		};
		std::vector<Component> m_Components;
	};
} // namespace brk::ecs
