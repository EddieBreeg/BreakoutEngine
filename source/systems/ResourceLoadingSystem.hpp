#pragma once

#include <PCH.hpp>
#include <ecs/System.hpp>

namespace brk {
	struct ResourceLoadRequestComponent;
	struct ResourceUnloadRequestComponent;

	/** WIP: This system takes care of all deferred resource loading/unloading requests.
	 * This will eventually be offloaded to a separate thread
	 */
	class ResourceLoadingSystem
	{
	public:
		using World = ecs::WorldView<
			const ResourceLoadRequestComponent,
			const ResourceUnloadRequestComponent>;

		ResourceLoadingSystem() = default;
		~ResourceLoadingSystem() = default;

		void Update(World& world, const TimeInfo&);

	private:
	};
} // namespace brk
