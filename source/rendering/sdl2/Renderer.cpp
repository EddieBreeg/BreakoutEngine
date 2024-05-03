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

void brk::rdr::Renderer::Shutdown()
{
	if (!m_Data)
		return;

#ifdef BRK_DEV
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
#endif

	SDL_DestroyRenderer(m_Data->m_NativeRenderer);

	delete m_Data;
	m_Data = nullptr;
}

#ifdef BRK_DEV
void brk::rdr::Renderer::NewImGuiFrame()
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}
#endif

void brk::rdr::Renderer::StartRender()
{
#ifdef BRK_DEV
	ImGuiIO& imguiIo = ImGui::GetIO();
	ImGui::Render();

	SDL_RenderSetScale(
		m_Data->m_NativeRenderer,
		imguiIo.DisplayFramebufferScale.x,
		imguiIo.DisplayFramebufferScale.y);
#endif
	SDL_SetRenderDrawColor(
		m_Data->m_NativeRenderer,
		static_cast<uint8>(m_ClearColor.x * 255),
		static_cast<uint8>(m_ClearColor.y * 255),
		static_cast<uint8>(m_ClearColor.z * 255),
		static_cast<uint8>(m_ClearColor.w * 255));
	SDL_RenderClear(m_Data->m_NativeRenderer);
}

void brk::rdr::Renderer::DoRender()
{
#ifdef BRK_DEV
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(m_Data->m_NativeRenderer);
#endif
}

brk::rdr::Renderer::~Renderer()
{
	Shutdown();
}