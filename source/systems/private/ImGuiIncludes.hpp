#pragma once

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>

#if defined(BRK_DX11)
#include <backends/imgui_impl_dx11.h>
#define IMGUI_BACKEND_INIT	ImGui_ImplSDL2_InitForD3D
#elif defined(BRK_VULKAN)
#include <backends/imgui_impl_vulkan.h>
#elif defined(BRK_OPENGL)
#include <backends/imgui_impl_opengl3.h>
#elif defined(BRK_SDL2_RENDERER)
#include <backends/imgui_impl_sdlrenderer2.h>
#else
#error "Unsupported rendering backend"
#endif