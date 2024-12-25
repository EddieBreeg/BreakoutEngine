#define _CRT_SECURE_NO_WARNINGS

#include "App.hpp"
#include "Entry.hpp"

#include <core/LogManager.hpp>
#include <core/ResourceLoader.hpp>
#include <ecs/ComponentRegistry.hpp>
#include <managers/ECSManager.hpp>
#include <managers/ResourceManager.hpp>
#include <managers/SceneManager.hpp>

#include <csignal>
#include <iostream>
#include <clocale>

#include <imgui/DebugOverlay.hpp>

#ifdef BRK_EDITOR
#include <editor/Editor.hpp>
#endif

#ifdef BRK_DEV
#include <filesystem>
#include <imgui.h>
#include <rendering/Renderer.hpp>
#endif

#include <SDL3/SDL_video.h>

#include <systems/VisualSystem.hpp>

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

	const char* GetImGuiIniFilePath()
	{
		static const std::string path = (GetAppDataPath() / "imgui.ini").string();
		return path.c_str();
	}
} // namespace

namespace brk {
	SDL_Window* WindowInit(const WindowSettings& settings, ImGuiContext& context);
	void WindowDestroy(SDL_Window* window);
	bool ProcessEvents(SDL_Window* window, ecs::EntityWorld& world);

	App::~App() {}

	void App::InitSystems(const EntryPoint& entryPoint)
	{
		{
			if (entryPoint.RegisterGameSystems)
				entryPoint.RegisterGameSystems(m_ECSManager);

#ifdef BRK_DEV
			m_ECSManager.AddSystem<VisualSystem>(*m_ImGuiContext);
#else
			m_ECSManager.AddSystem<VisualSystem>();
#endif
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

#if defined(BRK_DEV)
		rdr::Renderer::s_Instance.NewImGuiFrame();
		ImGui::DockSpaceOverViewport(1, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
		dbg::Overlay::s_Instance.Draw();
#endif

#ifdef BRK_EDITOR
		editor::Editor::GetInstance().Update();
		editor::Editor::GetInstance().ShowUI();
#endif

		m_ECSManager.Update(m_GameTime);

		m_ResourceLoader.ProcessBatch();
		m_GameTime.Update();
		return m_KeepRunning;
	}

	App::App(const EntryPoint& entry, const int argc, const char** argv)
		: m_Argc{ argc }
		, m_Argv{ argv }
#ifdef BRK_DEV
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
#ifdef BRK_DEV
			settings.m_Flags |= SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;
#else
			settings.m_Flags = SDL_WINDOW_FULLSCREEN;
#endif
			m_Window = WindowInit(settings, *m_ImGuiContext);

			rdr::Renderer::s_Instance.Init(*m_ImGuiContext, m_Window);
			rdr::Renderer::s_Instance.m_ClearColor = settings.m_ClearColor;
		}

#ifdef BRK_DEV
		ImGui::GetIO().IniFilename = GetImGuiIniFilePath();
		LogManager::GetInstance().m_Level = LogManager::Trace;
#endif

		InitManagers();

		ResourceManager& resManager = ResourceManager::GetInstance();
		RegisterResources(entry, resManager);
#ifdef BRK_DEV
		const auto& allocInfo = resManager.GetAllocTracker().GetInfo();
		BRK_LOG_TRACE(
			"Resource Manager is using {} bytes ({} allocs)",
			allocInfo.m_TotalSize,
			allocInfo.m_NumAllocs);
#endif

		InitSystems(entry);
		RegisterComponents(entry);
#ifdef BRK_EDITOR
		editor::Editor::Init(
			*m_ImGuiContext,
			ecs::Manager::GetInstance(),
			ResourceManager::GetInstance(),
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
#ifdef BRK_EDITOR
			editor::Editor,
#endif
			ResourceManager,
			SceneManager,
			ecs::Manager, // ensure the ecs manager is destroyed LAST, other managers
						  // might still need the entity world!
			ResourceLoader>();

		rdr::Renderer::s_Instance.Shutdown();
		WindowDestroy(m_Window);
	}
} // namespace brk
