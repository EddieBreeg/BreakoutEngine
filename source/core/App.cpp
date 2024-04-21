#include "App.hpp"

#include <systems/InputSystem.hpp>
#include <systems/VisualSystem.hpp>
#include <systems/WindowSystem.hpp>

#include <csignal>
#include <iostream>

namespace bre {

	App::~App() {}

	bool App::Update()
	{
		for (ecs::SystemInstance& system : m_Systems)
			system.Update(m_World, m_GameTime);
		m_GameTime.Update();
		return m_KeepRunning;
	}

	void App::InitSystems()
	{
		RegisterSystem<WindowSystem>();
		RegisterSystem<inputs::System>();
		// RegisterSystem<VisualSystem>();
	}

	App::App(const int argc, const char** argv)
		: m_Argc{ argc }
		, m_Argv{ argv }
	{}

	int App::Run()
	{
		signal(SIGINT,
			   [](int)
			   {
				   GetInstance().Terminate();
			   });
		InitSystems();
		while (m_KeepRunning)
			Update();

		m_Systems.clear();
		return 0;
	}
} // namespace bre
