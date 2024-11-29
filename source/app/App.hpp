#pragma once

#include <core/Singleton.hpp>
#include <core/TimeInfo.hpp>

namespace brk::ecs {
	class Manager;
}

namespace brk {
	struct EntryPoint;

	/**
	 * Application class, which represents the game/editor program
	 */
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

		void InitSystems(const EntryPoint& entryPoint);
		void InitManagers();
		void RegisterComponents(const EntryPoint& entryPoint);
		void RegisterResources(const EntryPoint& entryPoint);
		bool Update();

		void Cleanup();

		const int m_Argc;
		const char** const m_Argv;
		TimeInfo m_GameTime;

		ecs::Manager& m_ECSManager;
		bool m_KeepRunning = true;
	};
} // namespace brk
