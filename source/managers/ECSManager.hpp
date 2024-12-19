#pragma once

#include <PCH.hpp>

#include <core/AllocTracker.hpp>
#include <core/Singleton.hpp>
#include <core/TypeId.hpp>

#include <ecs/System.hpp>
#include <entt/entity/registry.hpp>

#include <vector>

namespace brk::ecs {
	/**
	 * Manages the ECS world and system instances
	 */
	class BRK_MANAGERS_API Manager : public brk::Singleton<Manager>
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
		void Update(const TimeInfo& timeInfo);
		[[nodiscard]] EntityWorld& GetWorld() noexcept { return m_EntityWorld; }

		void Clear();

		~Manager();

	private:
		static std::unique_ptr<Manager> s_Instance;
		friend class Singleton<Manager>;
		Manager();

		template <class T>
		static inline uint32 s_SysIndex = -1;

		EntityWorld m_EntityWorld;
		std::vector<SystemInstance> m_Systems;
	};
} // namespace brk::ecs

#include "ECSManager.inl"