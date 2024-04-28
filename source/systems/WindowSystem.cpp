#include <systems/WindowSystem.hpp>

#include <app/App.hpp>

#include <core/Assert.hpp>

#include <debug/DebugOverlay.hpp>

#ifdef BRK_EDITOR
#include <editor/Editor.hpp>
#endif

#include "private/ImGuiIncludes.hpp"

#include <SDL2/SDL.h>

namespace {
	template <brk::inputs::EEventType Evt, class... Args>
	inline entt::entity CreateInputEventEntity(
		brk::WindowSystem::World& world,
		Args&&... args)
	{
		entt::entity e = world.CreateEntity();
		world.AddComponent<brk::inputs::EventOneFrameComponent>(
			e,
			brk::TInputEvent::Create<Evt>(std::forward<Args>(args)...));
		return e;
	}
} // namespace

brk::WindowSystem::WindowSystem(const brk::WindowSystemSettings& settings)
	: m_Settings{ settings }
{
	const int initCode = SDL_Init(SDL_INIT_VIDEO);
	BRK_ASSERT(!initCode, "Failed to initialize SDL: {}", SDL_GetError());

	m_WinPtr = SDL_CreateWindow(
		m_Settings.m_Title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_Settings.m_Size.x,
		m_Settings.m_Size.y,
		m_Settings.m_Flags);
	BRK_ASSERT(m_WinPtr, "Failed to create window: {}", SDL_GetError());

#if defined(BRK_DEV)
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard |
								  ImGuiConfigFlags_DockingEnable |
								  ImGuiConfigFlags_ViewportsEnable;
	ImGuiStyle& theme = ImGui::GetStyle();
#if defined(BRK_SDL2_RENDERER)
	SDL_Renderer* renderer = SDL_CreateRenderer(m_WinPtr, -1, 0);
	ImGui_ImplSDL2_InitForSDLRenderer(m_WinPtr, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);
#else
#error "Unsupported"
#endif
#endif
}

void brk::WindowSystem::Terminate()
{
	if (!m_WinPtr)
		return;

#ifdef BRK_DEV
#if defined(BRK_SDL2_RENDERER)
	ImGui_ImplSDLRenderer2_Shutdown();
#endif

	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
#endif
	SDL_DestroyWindow(m_WinPtr);
	SDL_Quit();
}

void brk::WindowSystem::Update(World& world, const brk::TimeInfo& timeInfo)
{
#if defined(BRK_DEV)
#if defined(BRK_SDL2_RENDERER)
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
#endif
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
#endif

	ProcessEvents(world);

#if defined(BRK_DEV)
	dbg::Overlay::s_Instance.Draw();
#if defined(BRK_EDITOR)
	editor::Editor::GetInstance().ShowUI();
#endif

	ImGui::Render();
	auto& imguiIo = ImGui::GetIO();

#if defined(BRK_SDL2_RENDERER)
	SDL_Renderer* renderer = SDL_GetRenderer(m_WinPtr);
	SDL_RenderSetScale(
		renderer,
		imguiIo.DisplayFramebufferScale.x,
		imguiIo.DisplayFramebufferScale.y);
	SDL_SetRenderDrawColor(
		renderer,
		static_cast<uint8>(m_Settings.m_ClearColor.x * 255),
		static_cast<uint8>(m_Settings.m_ClearColor.y * 255),
		static_cast<uint8>(m_Settings.m_ClearColor.z * 255),
		static_cast<uint8>(m_Settings.m_ClearColor.w * 255));
	SDL_RenderClear(renderer);

	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(renderer);
#endif
	ImGui::EndFrame();

	// Update and Render additional Platform Windows
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
#endif
}

void brk::WindowSystem::ProcessEvents(World& world)
{
	using TEventQuery = ecs::query::Include<const inputs::EventOneFrameComponent>;
	// remove events from previous frame
	for (entt::entity event : world.Query<TEventQuery>())
		world.DestroyEntity(event);

	SDL_Event evt;

	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE: App::GetInstance().Terminate(); break;
		case SDL_MOUSEMOTION:
		{
			const int2 pos{ evt.motion.x, evt.motion.y };
			const int2 oldPos = pos - int2{ evt.motion.xrel, evt.motion.yrel };
			CreateInputEventEntity<inputs::MouseMove>(world, oldPos, pos);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
			CreateInputEventEntity<inputs::MouseButton>(
				world,
				evt.button.button,
				evt.button.state == SDL_PRESSED,
				evt.button.clicks,
				int2{ evt.button.x, evt.button.y });
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			CreateInputEventEntity<inputs::Key>(
				world,
				evt.key.keysym.sym,
				evt.key.keysym.mod,
				evt.key.state == SDL_PRESSED,
				evt.key.repeat);
			break;
		default: break;
		}

#if defined(BRK_DEV)
		ImGui_ImplSDL2_ProcessEvent(&evt);
#endif
	}
}