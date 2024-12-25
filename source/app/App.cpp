#define _CRT_SECURE_NO_WARNINGS

#define BRK_DEV (BRK_DEBUG || BRK_EDITOR)

#include "App.hpp"
#include "Entry.hpp"

#include <core/LogManager.hpp>
#include <core/ResourceLoader.hpp>
#include <ecs/ComponentRegistry.hpp>
#include <managers/ECSManager.hpp>
#include <managers/ResourceManager.hpp>
#include <managers/SceneManager.hpp>
#include <rendering/Renderer.hpp>
#include <SDL3/SDL_video.h>
#include <systems/VisualSystem.hpp>

#include <csignal>
#include <clocale>
#include <filesystem>
#include <iostream>

#if BRK_EDITOR
#include <editor/Editor.hpp>
#endif

#if BRK_DEV
#include <imgui/DebugOverlay.hpp>
#include <imgui/DevUiContext.hpp>
#include <imgui.h>
#include <systems/private/Init.hpp>
#endif

namespace {
	template <class... S>
	void DestroySingletons()
	{
		(S::Reset(), ...);
	}

	const std::filesystem::path& GetAppDataPath()
	{
		static const auto path =
			std::filesystem::path{ std::getenv(APPDATA) } / "BreakoutEngine";
		return path;
	}

#if BRK_DEV
	const char* GetImGuiIniFilePath()
	{
		static const std::string path = (GetAppDataPath() / "imgui.ini").string();
		return path.c_str();
	}
#endif
} // namespace

namespace brk {
	SDL_Window* WindowInit(const WindowSettings& settings);
	void WindowDestroy(SDL_Window* window);
	bool ProcessEvents(SDL_Window* window, ecs::EntityWorld& world);

	App::~App() {}

	void App::InitSystems(const EntryPoint& entryPoint)
	{
#if BRK_DEV
		systems::_internal::SetImGuiContext(*m_ImGuiContext);
#endif
		{
			if (entryPoint.RegisterGameSystems)
				entryPoint.RegisterGameSystems(m_ECSManager);

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
		ecs::EntityWorld& world = m_ECSManager.GetWorld();
		if (!(m_KeepRunning = ProcessEvents(m_Window, world)))
			return false;

#if BRK_DEV
		auto& devUiContext = dev_ui::Context::s_Instance;
		devUiContext.FrameStart();
		ImGui::DockSpaceOverViewport(1, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
#endif

#if BRK_EDITOR
		editor::Editor::GetInstance().Update();
#endif

		m_ECSManager.Update(m_GameTime);

#if BRK_EDITOR
		editor::Editor::GetInstance().ShowUI();
#endif
#if BRK_DEV
		dbg::Overlay::s_Instance.Draw();
		devUiContext.FrameEnd();

		devUiContext.Render();
		devUiContext.UpdateWindows();
#endif

		rdr::Renderer::s_Instance.Present();

		m_ResourceLoader.ProcessBatch();
		m_GameTime.Update();
		return m_KeepRunning;
	}

	App::App(const EntryPoint& entry, const int argc, const char** argv)
		: m_Argc{ argc }
		, m_Argv{ argv }
#if BRK_DEV
		, m_ImGuiContext{ ImGui::CreateContext() }
#endif
		, m_ECSManager{ ecs::Manager::Init() }
		, m_ResourceLoader{ ResourceLoader::Init() }
	{
		std::locale::global(std::locale("en_US", std::locale::all));

		const auto& appDataPath = GetAppDataPath();
		if (!std::filesystem::is_directory(appDataPath))
		{
			std::filesystem::create_directory(appDataPath);
		}

		{
			WindowSettings settings;
#if BRK_DEV
			settings.m_Flags |= SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;
#else
			settings.m_Flags = SDL_WINDOW_FULLSCREEN;
#endif
			m_Window = WindowInit(settings);

			rdr::Renderer::s_Instance.Init(m_Window);
			rdr::Renderer::s_Instance.m_ClearColor = settings.m_ClearColor;
		}

#if BRK_DEV
		ImGui::GetIO().IniFilename = GetImGuiIniFilePath();
		dev_ui::Context::s_Instance.Init(
			*m_ImGuiContext,
			m_Window,
			*brk::rdr::Renderer::s_Instance.GetData());
		LogManager::GetInstance().m_Level = LogManager::Trace;
#endif

		InitManagers();

		ResourceManager& resManager = ResourceManager::GetInstance();
		RegisterResources(entry, resManager);
#if BRK_DEBUG
		const auto& allocInfo = resManager.GetAllocTracker().GetInfo();
		BRK_LOG_TRACE(
			"Resource Manager is using {} bytes ({} allocs)",
			allocInfo.m_TotalSize,
			allocInfo.m_NumAllocs);
#endif

		InitSystems(entry);
		RegisterComponents(entry);
#if BRK_EDITOR
		editor::Editor::Init(
			*m_ImGuiContext,
			m_ECSManager,
			resManager,
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
		m_ResourceLoader.StopDeferred();
		ecs::Manager::GetInstance().Clear();

		DestroySingletons<
#if BRK_EDITOR
			editor::Editor,
#endif
			ResourceManager,
			SceneManager,
			ecs::Manager, // ensure the ecs manager is destroyed LAST, other managers
						  // might still need the entity world!
			ResourceLoader>();

#if BRK_DEV
		dev_ui::Context::s_Instance.Shutdown();
#endif
		rdr::Renderer::s_Instance.Shutdown();
		WindowDestroy(m_Window);
	}
} // namespace brk
