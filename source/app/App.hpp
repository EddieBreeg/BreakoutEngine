#pragma once

#include <core/TimeInfo.hpp>

struct ImGuiContext;
struct SDL_Window;

namespace brk {
	class ResourceManager;
}

namespace brk::ecs {
	class Manager;
}

namespace brk {
	class ResourceLoader;

	struct EntryPoint;

	/**
	 * Application class, which represents the game/editor program
	 */
	class App
	{
	public:
		~App();

		static App& Init(const EntryPoint& entry, const int argc, const char** argv)
		{
			if (s_Instance)
				return *s_Instance;
			s_Instance = new App{ entry, argc, argv };
			return *s_Instance;
		}
		static App& GetInstance() { return *s_Instance; }
		static void Reset()
		{
			delete s_Instance;
			s_Instance = nullptr;
		}

		/**
		 * Main loop. Blocks until Terminate is called.
		 */
		int Run();
		/**
		 * Requests the application to stop
		 */
		void Terminate() noexcept { m_KeepRunning = false; }

	private:
		static inline App* s_Instance = nullptr;
		/**
		 * Initializes the application with the provided command line arguments
		 */
		App(const EntryPoint& ep, const int argc, const char** argv);

		void InitSystems(const EntryPoint& entryPoint);
		void InitManagers();
		void RegisterComponents(const EntryPoint& entryPoint);
		void RegisterResources(const EntryPoint& entryPoint, ResourceManager& resManager);
		bool Update();

		void Cleanup();

		const int m_Argc;
		const char** const m_Argv;
		TimeInfo m_GameTime;
#if BRK_DEBUG || BRK_EDITOR
		ImGuiContext* m_ImGuiContext = nullptr;
#endif
		SDL_Window* m_Window = nullptr;

		ecs::Manager& m_ECSManager;
		ResourceLoader& m_ResourceLoader;
		bool m_KeepRunning = true;
	};
} // namespace brk
