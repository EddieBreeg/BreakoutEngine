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

		void InitEngineSystems();

		const int m_Argc;
		const char** const m_Argv;
		TimeInfo m_GameTime;

		ecs::Manager& m_ECSManager;
		bool m_KeepRunning = true;
	};
} // namespace brk
