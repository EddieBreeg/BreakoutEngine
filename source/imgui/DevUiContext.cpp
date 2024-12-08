#pragma once

#include "DevUiContext.hpp"

#ifdef BRK_DEV

#include <imgui.h>

brk::dev_ui::Context brk::dev_ui::Context::s_Instance;

void brk::dev_ui::Context::Init(ImGuiContext& ctx, SDL_Window* window)
{
	if (m_ImGuiContext)
		return;
	ImGui::SetCurrentContext(m_ImGuiContext = &ctx);
	m_Window = window;
}

#endif