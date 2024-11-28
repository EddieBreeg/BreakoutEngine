#include "Entry.hpp"

#include <core/LogManager.hpp>
#include <ecs/ComponentRegistry.hpp>
#include <managers/ECSManager.hpp>
#include <managers/ResourceManager.hpp>
#include <managers/SceneManager.hpp>

#include <csignal>
#include <iostream>
#include <clocale>

#include <debug/DebugOverlay.hpp>

#ifdef BRK_EDITOR
#include <editor/Editor.hpp>
#endif

#include <SDL3/SDL_video.h>

#include <systems/ResourceLoadingSystem.hpp>
#include <systems/VisualSystem.hpp>
#include <systems/WindowSystem.hpp>
#include "App.hpp"

namespace {
	void InitWindowSystem(brk::ecs::Manager& manager)
	{
		brk::WindowSystemSettings settings;
#ifdef BRK_DEV
		settings.m_Flags |= SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;
#else
		settings.m_Flags = SDL_WINDOW_FULLSCREEN;
#endif
		manager.AddSystem<brk::WindowSystem>(settings);
	}

	template <class... S>
	void DestroySingletons()
	{
		(S::Reset(), ...);
	}
} // namespace

namespace brk {

	App::~App() {}

	void App::InitSystems()
	{
		{
			m_ECSManager.AddSystem<ResourceLoadingSystem>();

			RegisterGameSystems(m_ECSManager);

			m_ECSManager.AddSystem<VisualSystem>();
		}
	}

	void App::InitManagers()
	{
		ResourceManager::Init(m_ECSManager.GetWorld());
		SceneManager::Init();
		ecs::ComponentRegistry::Init();
	}

	bool App::Update()
	{
		m_ECSManager.Update(m_GameTime);
#ifdef BRK_EDITOR
		editor::Editor::GetInstance().Update();
#endif
		m_GameTime.Update();
		return m_KeepRunning;
	}

	App::App(const int argc, const char** argv)
		: m_Argc{ argc }
		, m_Argv{ argv }
		, m_ECSManager{ ecs::Manager::Init() }
	{
		std::locale::global(std::locale("en_US", std::locale::all));
#ifdef BRK_DEV
		LogManager::GetInstance().m_Level = LogManager::Trace;
#endif
		InitManagers();
		// the window system needs to be initialized on his own, because it's responsible
		// for creating the renderer, which we may need to preload resources
		InitWindowSystem(m_ECSManager);

		RegisterResources();

		InitSystems();
		RegisterComponents();
#ifdef BRK_EDITOR
		editor::Editor::Init(
			ecs::Manager::GetInstance(),
			SceneManager::GetInstance(),
			m_Argc,
			m_Argv);
#endif
	}

	int App::Run()
	{
		signal(
			SIGINT,
			[](int)
			{
				GetInstance().Terminate();
			});

		while (m_KeepRunning)
			Update();

		Cleanup();

		return 0;
	}

	void App::Cleanup()
	{
		ecs::Manager::GetInstance().Clear();

		DestroySingletons<
#ifdef BRK_EDITOR
			editor::Editor,
#endif
			ResourceManager,
			SceneManager,
			ecs::Manager>(); // ensure the ecs manager is destroyed LAST, other managers
							 // might still need the entity world!
	}
} // namespace brk
