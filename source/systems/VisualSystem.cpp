#include "VisualSystem.hpp"
#ifdef BRK_DEV
#include <debug/DebugOverlay.hpp>
#include <editor/Editor.hpp>
#include <imgui.h>
#endif
#include <rendering/Renderer.hpp>

namespace brk {
	VisualSystem::VisualSystem() {}

	void VisualSystem::Update(World& world, const TimeInfo& timeInfo)
	{
		auto& renderer = rdr::Renderer::s_Instance;

		renderer.StartRender();
		renderer.DrawIndexed(3); // #hack, for testing purposes
		renderer.RenderUI();

#ifdef BRK_DEV
		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
#endif
		rdr::Renderer::s_Instance.Present();
	}

	void VisualSystem::Terminate()
	{
		rdr::Renderer::s_Instance.Shutdown();
	}
} // namespace brk
