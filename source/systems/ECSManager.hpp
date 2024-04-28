#pragma once

#include <PCH.hpp>

#include <core/Singleton.hpp>
#include <core/TypeId.hpp>

#include <ecs/System.hpp>

#include <vector>

namespace brk::ecs {
	class Manager : public brk::Singleton<Manager>
	{
	public:
		template <class S, class... Args>
		void AddSystem(Args&&... args);

		void Update(const TimeInfo& timeInfo);
		[[nodiscard]] entt::registry& GetWorld() noexcept { return m_EntityWorld; }

	private:
		friend class Singleton<Manager>;
		Manager();

		struct SystemTypeFamily;
		using SystemId = BasicTypeId<SystemTypeFamily>;

		std::vector<SystemInstance> m_Systems;
		entt::registry m_EntityWorld;
	};
} // namespace brk::ecs

#include "ECSManager.inl"