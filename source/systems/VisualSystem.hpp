#pragma once

#include <ecs/System.hpp>

namespace brk
{
	class TimeInfo;
}

namespace brk
{
	class VisualSystem
	{
	public:
		VisualSystem();
		~VisualSystem() { Terminate(); }

		void Terminate();

		using World = ecs::WorldView<>;

		BRK_ECS_UPDATE_DECL;
	private:
	};

} // namespace brk
