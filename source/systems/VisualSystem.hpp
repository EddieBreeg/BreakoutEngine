#pragma once

#include <ecs/System.hpp>

namespace brk {
	class TimeInfo;

	struct MeshComponent;
	struct VisualComponent;
} // namespace brk

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

		using World = ecs::WorldView<const MeshComponent, const VisualComponent>;

		void Update(World& world, const TimeInfo& timeInfo);

	private:
	};

} // namespace brk
