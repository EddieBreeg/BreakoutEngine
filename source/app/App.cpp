#include "App.hpp"

#include <debug/DebugOverlay.hpp>

#include <editor/Editor.hpp>

#include <systems/InputSystem.hpp>
#include <systems/VisualSystem.hpp>
#include <systems/WindowSystem.hpp>

#include <csignal>
#include <iostream>

namespace brk {

	App::~App() {}

	bool App::Update()
	{
		m_ECSManager.Update(m_GameTime);
		m_GameTime.Update();
		return m_KeepRunning;
	}

	App::App(const int argc, const char** argv)
		: m_Argc{ argc }
		, m_Argv{ argv }
		, m_ECSManager{ ecs::Manager::Init() }
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
#ifdef BRK_EDITOR
		Editor::Init(m_Argc, m_Argv);
#endif

		while (m_KeepRunning)
			Update();

		ecs::Manager::Reset();
		return 0;
	}
} // namespace brk
