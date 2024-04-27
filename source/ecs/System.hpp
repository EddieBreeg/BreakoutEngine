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

} // namespace brk::ecs

#define BRK_ECS_UPDATE_DECL                                                    \
	void Update(World& world, const brk::TimeInfo& timeInfo);

#include "System.inl"