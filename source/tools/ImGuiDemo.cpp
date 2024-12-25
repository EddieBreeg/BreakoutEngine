#include <PCH.hpp>

#if !BRK_DEV
int main(int argc, char const* argv[]) {}
#else

#include <backends/imgui_impl_sdl3.h>
#include <core/Assert.hpp>
#include <imgui.h>
#include <rendering/Renderer.hpp>
#include <SDL3/SDL.h>

namespace ImGui {
	void ShowDemoWindow(bool* p_open);
}

bool ProcessEvents(SDL_Window* window, brk::rdr::Renderer& renderer)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL3_ProcessEvent(&event);

		switch (event.type)
		{
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			if (event.window.windowID != SDL_GetWindowID(window))
				break;
			[[fallthrough]];
		case SDL_EVENT_QUIT: return false;
		case SDL_EVENT_WINDOW_RESIZED:
			renderer.ResizeFrameBuffers(
				uint32(event.window.data1),
				uint32(event.window.data2));
			break;

		default: break;
		}
	}
	return true;
}

int main(int argc, char const* argv[])
{
	ImGuiContext* context = ImGui::CreateContext();

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(
		"ImGui Demo",
		1280,
		720,
		SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

	BRK_ASSERT(window, "Failed to create window: {}", SDL_GetError());

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard |
					  ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

	using brk::rdr::Renderer;
	Renderer::s_Instance.Init(*context, window);

	while (ProcessEvents(window, Renderer::s_Instance))
	{
		Renderer::s_Instance.NewImGuiFrame();
		ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::ShowDemoWindow(nullptr);

		Renderer::s_Instance.StartRender();
		Renderer::s_Instance.RenderUI();

		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		Renderer::s_Instance.Present();
	}

	brk::rdr::Renderer::s_Instance.Shutdown();
	return 0;
}

#endif