#include "System.hpp"

namespace brk::ecs {
	template <class System, class... Args>
	inline SystemInstance ecs::SystemInstance::Create(Args&&... args)
	{
		SystemInstance r;
		System::Init(std::forward<Args>(args)...);

		r.m_Update = [](entt::registry& world, const TimeInfo& timeInfo)
		{
			static_assert(IsWorldView<typename System::World>,
						  "The System::World isn't a valid world view type");
			typename System::World worldView{ world };
			System::GetInstance().Update(worldView, timeInfo);
		};
		r.m_Terminate = &Terminate<System>::Impl;

		return r;
	}
} // namespace brk::ecs
