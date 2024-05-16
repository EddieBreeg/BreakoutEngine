#pragma once

#include <PCH.hpp>
#include <ecs/System.hpp>

namespace brk {
	struct ResourceLoadRequestComponent;
	struct ResourceUnloadRequestComponent;

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
