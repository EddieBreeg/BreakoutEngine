#pragma once

#include <PCH.hpp>
#if BRK_DEBUG
#include <core/AllocTracker.hpp>
#endif
#include <entt/entity/fwd.hpp>


namespace brk::ecs {
#if BRK_DEBUG
	using EntityWorld =
		entt::basic_registry<entt::entity, TrackerAllocator<entt::entity>>;
#else
	using EntityWorld = entt::basic_registry<entt::entity>;
#endif
} // namespace brk::ecs
