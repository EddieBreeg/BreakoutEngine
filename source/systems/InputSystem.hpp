#pragma once

#include <PCH.hpp>

#include <core/Singleton.hpp>

#include <ecs/System.hpp>
#include <ecs/World.hpp>

namespace brk::inputs {
	struct EventOneFrameComponent;
} // namespace brk::inputs

namespace brk::inputs {

	class System : public Singleton<System>
	{
	public:
		using World = ecs::WorldView<const inputs::EventOneFrameComponent>;
		BRK_ECS_UPDATE_DECL;

		~System() = default;

	private:
		void ProcessEvents(World& world);
		friend class Singleton<System>;
		System() = default;
	};

} // namespace brk::inputs
