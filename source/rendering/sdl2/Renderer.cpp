#include <rendering/Renderer.hpp>
#include <core/Assert.hpp>
#include <SDL2/SDL.h>

#ifdef BRK_DEV
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>
#endif

struct brk::rdr::RendererData
{
	SDL_Renderer* m_NativeRenderer = nullptr;
};

void brk::rdr::Renderer::Init(SDL_Window* window)
{
	m_Window = window;
	BRK_ASSERT(m_Window, "Trying to initialize renderer with invalid window pointer!");

	SDL_Renderer* renderer = SDL_CreateRenderer(m_Window, -1, 0);
	BRK_ASSERT(renderer, "Failed to create SDL renderer");

	m_Data = new RendererData{ renderer };

#ifdef BRK_DEV
	ImGui_ImplSDL2_InitForSDLRenderer(m_Window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);
#endif
}

brk::rdr::Renderer::~Renderer()
{
	if (!m_Data)
		return;

	delete m_Data;
	m_Data = nullptr;
}