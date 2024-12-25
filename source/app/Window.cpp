#define BRK_DEV (BRK_DEBUG || BRK_EDITOR)

#include "Entry.hpp"

#include <core/Assert.hpp>
#include <core/Event.hpp>
#include <ecs/World.hpp>
#include <entt/entity/registry.hpp>

#if BRK_DEV
#include <backends/imgui_impl_sdl3.h>
#include <imgui.h>
#endif

#if BRK_EDITOR
#include <editor/Editor.hpp>
#endif

#include <rendering/Renderer.hpp>
#include <systems/InputEventTypes.hpp>
#include <SDL3/SDL.h>

namespace {
	template <brk::inputs::EEventType Evt, class... Args>
	inline entt::entity CreateInputEventEntity(
		brk::ecs::EntityWorld& world,
		Args&&... args)
	{
		entt::entity e = world.create();
		world.emplace<brk::inputs::EventOneFrameComponent>(
			e,
			brk::TInputEvent::Create<Evt>(std::forward<Args>(args)...));
		return e;
	}
} // namespace

namespace brk {
	SDL_Window* WindowInit(const WindowSettings& settings)
	{
		const int initCode = SDL_Init(SDL_INIT_VIDEO);
		BRK_ASSERT(initCode, "Failed to initialize SDL: {}", SDL_GetError());

		SDL_Window* window = SDL_CreateWindow(
			settings.m_Title,
			settings.m_Size.x,
			settings.m_Size.y,
			settings.m_Flags);
		BRK_ASSERT(window, "Failed to create window: {}", SDL_GetError());

#if BRK_DEV
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard |
						  ImGuiConfigFlags_DockingEnable |
						  ImGuiConfigFlags_ViewportsEnable;
		const float uiScale = SDL_GetDisplayContentScale(SDL_GetDisplayForWindow(window));
		BRK_ASSERT(uiScale, "Failed to retrieve DPI scaling value: {}", SDL_GetError());

#if BRK_EDITOR
		brk::editor::ui::ImportEditorFonts(io, uiScale);
#endif

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::StyleColorsDark(&style);
		style.ScaleAllSizes(uiScale);
#endif

		return window;
	}

	void WindowDestroy(SDL_Window* window)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	bool ProcessEvents(SDL_Window* window, ecs::EntityWorld& world)
	{
		SDL_Event evt;
		const SDL_WindowID mainWindowId = SDL_GetWindowID(window);

		{
			auto view = world.view<const inputs::EventOneFrameComponent>();
			for (auto&& event : view)
				world.destroy(event);
		}

		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				if (evt.window.windowID != mainWindowId)
					break;
				[[fallthrough]];
			case SDL_EVENT_QUIT: return false;

			case SDL_EVENT_WINDOW_RESIZED:
				if (evt.window.windowID == mainWindowId)
				{
					rdr::Renderer::s_Instance.ResizeFrameBuffers(
						uint32(evt.window.data1),
						uint32(evt.window.data2));
				}
				break;
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

#if BRK_DEV
			ImGui_ImplSDL3_ProcessEvent(&evt);
#endif
		}
		return true;
	}
} // namespace brk
