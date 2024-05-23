#pragma once

#include <ecs/World.hpp>

namespace brk {
	class TimeInfo;
} // namespace brk

namespace brk::ecs {
	/**
	 * An ECS system instance.
	 */
	class SystemInstance
	{
	public:
		SystemInstance(const SystemInstance&) = delete;
		SystemInstance(SystemInstance&& other);
		/**
		 * Updates the underlying system
		 */
		void Update(entt::registry& world, const TimeInfo& timeInfo);

		/**
		 * Creates an instance for the provided system type.
		 * \note System must meet the following criteria:
		 * - Have an accessible specialization of WorldView called World
		 * - Have an Update function, callable with a System::World& and a const TimeInfo&
		 * \param args The arguments which will be forwarded to the system's constructor
		 */
		template <class System, class... Args>
		static SystemInstance Create(Args&&... args);

		~SystemInstance();

	private:
		SystemInstance() = default;

		void* m_SystemPtr = nullptr;
		void (*m_Update)(void* ptr, entt::registry&, const TimeInfo&) = nullptr;
		void (*m_Terminate)(void* ptr) = nullptr;
	};

} // namespace brk::ecs

#include "System.inl"