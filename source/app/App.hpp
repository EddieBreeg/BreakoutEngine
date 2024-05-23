#pragma once

#include <core/Singleton.hpp>
#include <core/TimeInfo.hpp>

#include <managers/ECSManager.hpp>

#include <entt/entity/registry.hpp>

#include <vector>

namespace brk {
	class App : public Singleton<App>
	{
	public:
		~App();

		/**
		 * Main loop. Blocks until Terminate is called.
		 */
		int Run();
		/**
		 * Requests the application to stop
		 */
		void Terminate() noexcept { m_KeepRunning = false; }

	private:
		/**
		 * Initializes the application with the provided command line arguments
		 */
		friend class Singleton<App>;
		App(const int argc, const char** argv);

		void InitCoreSystems();
		void InitManagers();
		bool Update();

		const int m_Argc;
		const char** const m_Argv;
		TimeInfo m_GameTime;

		ecs::Manager& m_ECSManager;
		bool m_KeepRunning = true;
	};
} // namespace brk
