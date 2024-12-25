#pragma once

#include <ecs/System.hpp>

namespace brk {
	class TimeInfo;

	struct MeshComponent;
	struct TransformComponent;
	struct VisualComponent;
} // namespace brk

namespace brk {
	/**
	 * Rendering system
	 */
	class BRK_SYSTEMS_API VisualSystem
	{
	public:
		VisualSystem() = default;

		~VisualSystem() = default;

		using World = ecs::WorldView<const MeshComponent, const TransformComponent>;

		void Update(World& world, const TimeInfo& timeInfo);
	private:
	};

} // namespace brk
