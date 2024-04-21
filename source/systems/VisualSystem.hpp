#pragma once

#include <core/Singleton.hpp>
#include <ecs/System.hpp>

namespace bre {
	class TimeInfo;
}

namespace bre {
	class VisualSystem : public Singleton<VisualSystem>
	{
	public:
		void Terminate();

		~VisualSystem();

	private:
		friend class Singleton<VisualSystem>;
		VisualSystem();
	};

} // namespace bre
