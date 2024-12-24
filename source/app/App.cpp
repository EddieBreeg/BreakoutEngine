#define _CRT_SECURE_NO_WARNINGS

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
#endif

#include <SDL3/SDL_video.h>

#include <systems/VisualSystem.hpp>
#include <systems/WindowSystem.hpp>
#include "App.hpp"

namespace {
	void InitWindowSystem(brk::App& app, brk::ecs::Manager& manager)
	{
		brk::WindowSystemSettings settings;
#ifdef BRK_DEV
		settings.m_Flags |= SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED;
#else
		settings.m_Flags = SDL_WINDOW_FULLSCREEN;
#endif
		manager.AddSystem<brk::WindowSystem>(app, settings);
	}

	template <class... S>
	void DestroySingletons()
	{
		(S::Reset(), ...);
	}

	const std::filesystem::path& GetAppDataPath()
	{
		static const auto path = std::filesystem::path{ std::getenv(APPDATA) } / "BreakoutEngine";
		return path;
	}

	const char* GetImGuiIniFilePath()
	{
		static const std::string path = (GetAppDataPath() / "imgui.ini").string();
		return path.c_str();
	}
} // namespace

namespace brk {
	App::~App() {}

	void App::InitSystems(const EntryPoint& entryPoint)
	{
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
		m_ECSManager.Update(m_GameTime);
#ifdef BRK_EDITOR
		editor::Editor::GetInstance().Update();
#endif
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

#ifdef BRK_DEV
		ImGui::GetIO().IniFilename = GetImGuiIniFilePath();
		LogManager::GetInstance().m_Level = LogManager::Trace;
#endif
		InitManagers();
		// the window system needs to be initialized on his own, because it's responsible
		// for creating the renderer, which we may need to preload resources
		InitWindowSystem(*this, m_ECSManager);

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
	}
} // namespace brk
