#include "Entry.hpp"

#include <core/LogManager.hpp>
#include <ecs/ComponentRegistry.hpp>
#include <managers/ResourceManager.hpp>
#include <managers/SceneManager.hpp>

#include <csignal>
#include <iostream>

#include <debug/DebugOverlay.hpp>

#ifdef BRK_EDITOR
#include <editor/Editor.hpp>
#endif

#include <SDL2/SDL_video.h>

#include <systems/ResourceLoadingSystem.hpp>
#include <systems/VisualSystem.hpp>
#include <systems/WindowSystem.hpp>
#include "App.hpp"

namespace brk {

	App::~App() {}

	void App::InitSystems()
	{
		{
			WindowSystemSettings settings;
#ifdef BRK_DEV
			settings.m_Flags |= SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;
#else
			settings.m_Flags = SDL_WINDOW_FULLSCREEN;
#endif
			m_ECSManager.AddSystem<WindowSystem>(settings);
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
#ifdef BRK_DEV
		LogManager::GetInstance().m_Level = LogManager::Trace;
#endif
		InitManagers();
		InitSystems();
		RegisterComponents();
#ifdef BRK_EDITOR
		editor::Editor::Init(m_Argc, m_Argv);
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

		ecs::Manager::Reset();
#ifdef BRK_EDITOR
		editor::Editor::Reset();
#endif
		return 0;
	}
} // namespace brk
