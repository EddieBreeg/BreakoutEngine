#pragma once

#include <PCH.hpp>

#include <core/Singleton.hpp>

#include <ecs/System.hpp>
#include <ecs/World.hpp>

namespace bre::inputs {
	struct EventOneFrameComponent;
} // namespace bre::inputs

namespace bre::inputs {

	class System : public Singleton<System>
	{
	public:
		using World = ecs::Include<const inputs::EventOneFrameComponent>::ViewType;
		BREAKOUT_ECS_UPDATE_DECL;

		~System() = default;

	private:
		void ProcessEvents(World& world);
		friend class Singleton<System>;
		System() = default;
	};

} // namespace bre::inputs
