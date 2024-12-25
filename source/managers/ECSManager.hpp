#pragma once

#include <PCH.hpp>

#include <core/Singleton.hpp>
#include <core/TypeId.hpp>

#include <ecs/System.hpp>
#include <entt/entity/registry.hpp>

#include <vector>

namespace brk::ecs {
	/**
	 * Manages the ECS world and system instances
	 */
	class Manager : public brk::Singleton<Manager>
	{
	public:
		/**
		 * Creates a new instance for the system, and adds it to the manager
		 * \tparam S The system type
		 * \param args The arguments which will be passed to the system's constructor
		 * \note Will assert if the system had already been registered
		 */
		template <class S, class... Args>
		void AddSystem(Args&&... args);

		/**
		 * Updates all systems
		 */
		BRK_MANAGERS_API void Update(const TimeInfo& timeInfo);
		[[nodiscard]] EntityWorld& GetWorld() noexcept { return m_EntityWorld; }

		/**
		 * Terminates all ECS systems, clears the entity world.
		 */
		BRK_MANAGERS_API void Clear();

		BRK_MANAGERS_API ~Manager();

	private:
		friend class Singleton<Manager>;

		BRK_MANAGERS_API static std::unique_ptr<Manager> s_Instance;
		BRK_MANAGERS_API Manager();

		template <class T>
		static inline uint32 s_SysIndex = uint32(-1);

#if BRK_DEBUG
		AllocTracker m_AllocTracker;
#endif
		EntityWorld m_EntityWorld;
		std::vector<SystemInstance> m_Systems;
	};
} // namespace brk::ecs

#include "ECSManager.inl"