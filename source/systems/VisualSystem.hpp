#pragma once

#include <core/Singleton.hpp>
#include <ecs/System.hpp>

namespace breakout {
	class TimeInfo;
}

namespace breakout {
	class VisualSystem : public Singleton<VisualSystem>
	{
	public:
		void Terminate();

		~VisualSystem();

	private:
		friend class Singleton<VisualSystem>;
		VisualSystem();
	};

} // namespace breakout
