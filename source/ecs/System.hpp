#pragma once

#include <ecs/World.hpp>

namespace brk {
	template <class T>
	class Singleton;

	class TimeInfo;
} // namespace brk

namespace brk::ecs {
	class SystemInstance
	{
	public:
		SystemInstance(const SystemInstance&) = delete;
		SystemInstance(SystemInstance&& other);
		void Update(entt::registry& world, const TimeInfo& timeInfo);

		template <class System, class... Args>
		static SystemInstance Create(Args&&... args);

		~SystemInstance();

	private:
		SystemInstance() = default;

		void *m_SystemPtr = nullptr;
		void (*m_Update)(void* ptr, entt::registry&, const TimeInfo&) = nullptr;
		void (*m_Terminate)(void* ptr) = nullptr;
	};

} // namespace brk::ecs

#include "System.inl"