#include "Renderer.hpp"

#ifdef BRK_DEV
#include <imgui.h>

void brk::rdr::Renderer::EndImGuiFrame()
{
	ImGui::EndFrame();
}
#endif

brk::rdr::Renderer brk::rdr::Renderer::s_Instance;
