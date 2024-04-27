#pragma once

#include <core/Singleton.hpp>
#include <ecs/System.hpp>

namespace brk {
	class TimeInfo;
}

namespace brk {
	class VisualSystem : public Singleton<VisualSystem>
	{
	public:
		void Terminate();

		~VisualSystem();

	private:
		friend class Singleton<VisualSystem>;
		VisualSystem();
	};

} // namespace brk
