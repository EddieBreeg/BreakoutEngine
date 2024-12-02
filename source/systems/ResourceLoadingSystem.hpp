#pragma once

#include <PCH.hpp>
#include <core/Singleton.hpp>
#include <ecs/System.hpp>
#include <vector>

namespace brk {
	/** WIP: This system takes care of all deferred resource loading/unloading requests.
	 * This will eventually be offloaded to a separate thread
	 */
	class BRK_SYSTEMS_API ResourceLoadingSystem
	{
	public:
		using World = ecs::WorldView<>;

		ResourceLoadingSystem() = default;
		~ResourceLoadingSystem() = default;

		void Update(World& world, const TimeInfo&);

	private:
	};
} // namespace brk
