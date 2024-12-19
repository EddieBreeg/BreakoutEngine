#pragma once

#include <PCH.hpp>
#ifdef BRK_DEV
#include <core/AllocTracker.hpp>
#endif
#include <entt/entity/fwd.hpp>


namespace brk::ecs {
#ifdef BRK_DEV
	using EntityWorld =
		entt::basic_registry<entt::entity, TrackerAllocator<entt::entity>>;
#else
	using EntityWorld = entt::basic_registry<entt::entity>;
#endif
} // namespace brk::ecs
