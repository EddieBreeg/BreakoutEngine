#pragma once

#include <ecs/System.hpp>

namespace brk {
	class TimeInfo;
}

namespace brk {
	/**
	 * Rendering system
	 */
	class VisualSystem
	{
	public:
		VisualSystem();
		~VisualSystem() { Terminate(); }

		void Terminate();

		using World = ecs::WorldView<>;

		void Update(World& world, const TimeInfo& timeInfo);

	private:
	};

} // namespace brk
