#pragma once

#include <core/Singleton.hpp>
#include <core/TimeInfo.hpp>

#include <ecs/System.hpp>
#include <ecs/ECSManager.hpp>

#include <entt/entity/registry.hpp>

#include <vector>

namespace brk {
	class App : public Singleton<App>
	{
	public:
		App(const int argc, const char** argv);
		~App();

		int Run();
		void Terminate() noexcept { m_KeepRunning = false; }

	private:
		bool Update();

		template <class T, class... Args>
		void RegisterSystem(Args&&... args)
		{
			m_Systems.emplace_back(
				ecs::SystemInstance::Create<T>(std::forward<Args>(args)...));
		}

		void InitEngineSystems();

		const int m_Argc;
		const char** const m_Argv;
		entt::registry m_World;
		TimeInfo m_GameTime;
		std::vector<ecs::SystemInstance> m_Systems;
		bool m_KeepRunning = true;
	};
} // namespace brk
