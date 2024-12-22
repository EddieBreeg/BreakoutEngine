#include <systems/WindowSystem.hpp>

#include <app/App.hpp>

#include <core/Assert.hpp>
#include <entt/entity/registry.hpp>

#include <imgui/DebugOverlay.hpp>
#include <imgui/DevUiContext.hpp>

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

#ifdef BRK_EDITOR
namespace brk::editor {
	void ImportEditorFonts(ImGuiIO& io, float dpiScale);
}
#endif

brk::WindowSystem::WindowSystem(App& app, const brk::WindowSystemSettings& settings)
	: m_Settings{ settings }
	, m_App{ app }
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
	ImGuiContext& context = m_App.GetImGuiContext();
	ImGui::SetCurrentContext(&context);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard |
					  ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
	const float uiScale = SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(m_WinPtr));
	BRK_ASSERT(uiScale, "Failed to retrieve DPI scaling value: {}", SDL_GetError());
	dev_ui::Context::s_Instance.Init(m_App.GetImGuiContext(), m_WinPtr);
#ifdef BRK_EDITOR
	editor::ImportEditorFonts(io, uiScale);
#endif
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::StyleColorsDark(&style);
	style.ScaleAllSizes(uiScale);
#endif

	rdr::Renderer::s_Instance.Init(context, m_WinPtr);
	rdr::Renderer::s_Instance.m_ClearColor = m_Settings.m_ClearColor;
}

void brk::WindowSystem::Terminate()
{
	if (!m_WinPtr)
		return;

	SDL_DestroyWindow(m_WinPtr);
	SDL_Quit();
}

void brk::WindowSystem::Update(World& world, const brk::TimeInfo& timeInfo)
{
#if defined(BRK_DEV)
	rdr::Renderer::s_Instance.NewImGuiFrame();
	ImGui::DockSpaceOverViewport(1, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
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
	const SDL_WindowID mainWindowId = SDL_GetWindowID(m_WinPtr);

	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			if (evt.window.windowID == mainWindowId)
				m_App.Terminate();
			break;
		case SDL_EVENT_WINDOW_RESIZED:
			if (evt.window.windowID == mainWindowId)
			{
				rdr::Renderer::s_Instance.ResizeFrameBuffers(
					uint32(evt.window.data1),
					uint32(evt.window.data2));
			}
			break;
		case SDL_EVENT_QUIT: m_App.Terminate(); break;
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