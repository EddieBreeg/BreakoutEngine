#pragma once

#include <PCH.hpp>

#include <core/Singleton.hpp>
#include <core/TypeId.hpp>

#include <ecs/System.hpp>

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
		void Update(const TimeInfo& timeInfo);
		[[nodiscard]] entt::registry& GetWorld() noexcept { return m_EntityWorld; }

	private:
		friend class Singleton<Manager>;
		Manager();

		struct SystemTypeFamily;
		using SystemId = BasicTypeId<SystemTypeFamily>;

		template <class T>
		static inline uint32 s_SysIndex = -1;

		std::vector<SystemInstance> m_Systems;
		entt::registry m_EntityWorld;
	};
} // namespace brk::ecs

#include "ECSManager.inl"