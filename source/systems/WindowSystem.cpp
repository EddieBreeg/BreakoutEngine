#include <systems/WindowSystem.hpp>

#include <app/App.hpp>

#include <core/Assert.hpp>

#include <debug/DebugOverlay.hpp>

#ifdef BRK_EDITOR
#include <editor/Editor.hpp>
#endif

#include <rendering/Renderer.hpp>

#include "private/ImGuiIncludes.hpp"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>

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
	BRK_ASSERT(initCode, "Failed to initialize SDL: {}", SDL_GetError());

	m_WinPtr = SDL_CreateWindow(
		m_Settings.m_Title,
		m_Settings.m_Size.x,
		m_Settings.m_Size.y,
		m_Settings.m_Flags);
	BRK_ASSERT(m_WinPtr, "Failed to create window: {}", SDL_GetError());

#if defined(BRK_DEV)
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard |
								  ImGuiConfigFlags_DockingEnable |
								  ImGuiConfigFlags_ViewportsEnable;
#endif

	rdr::Renderer::s_Instance.Init(m_WinPtr);
	rdr::Renderer::s_Instance.m_ClearColor = m_Settings.m_ClearColor;
}

void brk::WindowSystem::Terminate()
{
	if (!m_WinPtr)
		return;

	rdr::Renderer::s_Instance.Shutdown();
	SDL_DestroyWindow(m_WinPtr);
	SDL_Quit();
}

void brk::WindowSystem::Update(World& world, const brk::TimeInfo& timeInfo)
{
#if defined(BRK_DEV)
	rdr::Renderer::s_Instance.NewImGuiFrame();
	ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
#endif

	ProcessEvents(world);

#if defined(BRK_DEV)
	dbg::Overlay::s_Instance.Draw();
#endif
#if defined(BRK_EDITOR)
	editor::Editor::GetInstance().ShowUI();
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
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED: App::GetInstance().Terminate(); break;
		case SDL_EVENT_WINDOW_RESIZED:
			rdr::Renderer::s_Instance.ResizeFrameBuffers(
				uint32(evt.window.data1),
				uint32(evt.window.data2));
			break;
		case SDL_EVENT_QUIT: App::GetInstance().Terminate(); break;
		case SDL_EVENT_MOUSE_MOTION:
		{
			const int2 pos{ evt.motion.x, evt.motion.y };
			const int2 oldPos = pos - int2{ evt.motion.xrel, evt.motion.yrel };
			CreateInputEventEntity<inputs::MouseMove>(world, oldPos, pos);
			break;
		}
		case SDL_EVENT_MOUSE_BUTTON_UP:
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			CreateInputEventEntity<inputs::MouseButton>(
				world,
				evt.button.button,
				evt.button.down,
				evt.button.clicks,
				int2{ evt.button.x, evt.button.y });
			break;
		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN:
			CreateInputEventEntity<inputs::Key>(
				world,
				evt.key.key,
				evt.key.mod,
				evt.key.down,
				evt.key.repeat);
			break;
		default: break;
		}

#if defined(BRK_DEV)
		ImGui_ImplSDL3_ProcessEvent(&evt);
#endif
	}
}