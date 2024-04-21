#pragma once

#include <ecs/World.hpp>

namespace bre {
	template <class T>
	class Singleton;

	class TimeInfo;
} // namespace bre

namespace bre::ecs {
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
		template <class System, class = void>
		struct Terminate
		{
			static void Impl() noexcept {};
		};
		template <class System>
		struct Terminate<System,
						 std::enable_if_t<std::is_member_function_pointer_v<
							 decltype(&System::Terminate)>>>
		{
			static void Impl() { System::GetInstance().Terminate(); };
		};

		SystemInstance() = default;

		void (*m_Update)(entt::registry&, const TimeInfo&) = nullptr;
		void (*m_Terminate)() = nullptr;
	};

} // namespace bre::ecs

#define BREAKOUT_ECS_UPDATE_DECL                                                    \
	void Update(World& world, const bre::TimeInfo& timeInfo);

#include "System.inl"