#include "DebugOverlay.hpp"
#ifdef BRK_DEV
#include <imgui.h>

brk::dbg::Overlay brk::dbg::Overlay::s_Instance;

void ImGui::ShowDemoWindow(bool* p_open);

void brk::dbg::Overlay::Draw()
{
	if (!m_Enabled)
		return;
	ImGui::ShowDemoWindow(&m_Enabled);
}

void brk::dbg::Overlay::Init(ImGuiContext& context)
{
	ImGui::SetCurrentContext(&context);
}

#endif