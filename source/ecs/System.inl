namespace brk::ecs
{
	template <class System, class... Args>
	SystemInstance ecs::SystemInstance::Create(Args&&... args)
	{
		SystemInstance r;

		System* ptr = new System{ std::forward<Args>(args)... };
		r.m_SystemPtr = ptr;
		r.m_Update = [](void* ptr, entt::registry& world, const TimeInfo& timeInfo)
		{
			static_assert(
				IsWorldView<typename System::World>,
				"System::World isn't a valid world view type");
			typename System::World worldView{ world };
			reinterpret_cast<System*>(ptr)->Update(worldView, timeInfo);
		};

		r.m_Terminate = [](void* ptr)
		{
			delete reinterpret_cast<System*>(ptr);
		};

		return r;
	}
} // namespace brk::ecs
