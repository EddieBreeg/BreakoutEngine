#pragma once

#include "DevUiContext.hpp"

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#ifdef BRK_DX11
#include <rendering/dx11/Renderer.hpp>
#include <backends/imgui_impl_dx11.h>
#endif

brk::dev_ui::Context brk::dev_ui::Context::s_Instance;

void brk::dev_ui::Context::Init(
	ImGuiContext& ctx,
	SDL_Window* window,
	rdr::RendererData& data)
{
	if (m_ImGuiContext)
		return;

	ImGui::SetCurrentContext(m_ImGuiContext = &ctx);
	m_Window = window;

#ifdef BRK_DX11
	ImGui_ImplSDL3_InitForD3D(window);
	ImGui_ImplDX11_Init(data.m_Device, data.m_DeviceContext);
#endif
}

void brk::dev_ui::Context::Shutdown()
{
#ifdef BRK_DX11
	ImGui_ImplDX11_Shutdown();
#endif
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}

void brk::dev_ui::Context::FrameStart()
{
#ifdef BRK_DX11
	ImGui_ImplDX11_NewFrame();
#endif
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void brk::dev_ui::Context::FrameEnd()
{
	ImGui::Render();
}

void brk::dev_ui::Context::Render()
{
#if BRK_DX11
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
}

void brk::dev_ui::Context::UpdateWindows()
{
	if (!(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable))
		return;

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}
