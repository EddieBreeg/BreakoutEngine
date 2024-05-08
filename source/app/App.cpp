#include "App.hpp"

#include <core/LogManager.hpp>
#include <ecs/SceneManager.hpp>

#include <csignal>
#include <iostream>

#include <debug/DebugOverlay.hpp>

#include <editor/Editor.hpp>

#include <SDL2/SDL_video.h>

#include <systems/VisualSystem.hpp>
#include <systems/WindowSystem.hpp>

namespace brk {

	App::~App() {}

	void App::InitCoreSystems()
	{
		{
			WindowSystemSettings settings;
#ifdef BRK_DEV
			settings.m_Flags |= SDL_WINDOW_RESIZABLE;
#endif
			m_ECSManager.AddSystem<WindowSystem>(settings);
		}
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
		InitCoreSystems();
#ifdef BRK_EDITOR
		editor::Editor::Init(m_Argc, m_Argv);
#endif
		m_ECSManager.AddSystem<VisualSystem>();

		ecs::SceneManager::Init();
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
