#include "VisualSystem.hpp"
#ifdef BRK_DEV
#include <debug/DebugOverlay.hpp>
#include <editor/Editor.hpp>
#include <imgui.h>
#endif
#include <rendering/Renderer.hpp>

namespace brk
{
	VisualSystem::VisualSystem() {}

	void VisualSystem::Update(World& world, const TimeInfo& timeInfo)
	{
		rdr::Renderer::s_Instance.StartRender();
		rdr::Renderer::s_Instance.RenderUI();

#ifdef BRK_DEV
		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		// ImGui::EndFrame();
#endif
		rdr::Renderer::s_Instance.Present();
	}

	void VisualSystem::Terminate()
	{
		rdr::Renderer::s_Instance.Shutdown();
	}
} // namespace brk
