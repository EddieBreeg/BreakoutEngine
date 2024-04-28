#include "App.hpp"

#include <debug/DebugOverlay.hpp>

#include <systems/InputSystem.hpp>
#include <systems/VisualSystem.hpp>
#include <systems/WindowSystem.hpp>

#include <SDL2/SDL.h>

#include <csignal>
#include <iostream>

namespace brk {

	App::~App() {}

	bool App::Update()
	{
		for (ecs::SystemInstance& system : m_Systems)
			system.Update(m_World, m_GameTime);
		m_GameTime.Update();
		return m_KeepRunning;
	}

	void App::InitEngineSystems()
	{
		{
			WindowSystemSettings settings;
#ifdef BRK_DEV
			settings.m_Flags |= SDL_WINDOW_RESIZABLE;
#endif
			RegisterSystem<WindowSystem>(settings);
		}
		RegisterSystem<inputs::System>();
	}

	App::App(const int argc, const char** argv)
		: m_Argc{ argc }
		, m_Argv{ argv }
	{}

	int App::Run()
	{
		signal(
			SIGINT,
			[](int)
			{
				GetInstance().Terminate();
			});
#if defined(BRK_DEBUG)
		dbg::Overlay::s_Instance.m_Enabled = true;
#endif
		InitEngineSystems();
		while (m_KeepRunning)
			Update();

		m_Systems.clear();
		return 0;
	}
} // namespace brk
