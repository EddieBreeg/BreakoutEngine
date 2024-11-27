#include "VisualSystem.hpp"

#include "VisualComponents.hpp"
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

		auto meshes = world.Query<ecs::query::Include<MeshComponent>>();
		for (entt::entity m : meshes)
		{
			MeshComponent& comp = meshes.Get<MeshComponent>(m);
			renderer.SetMaterial(*comp.m_MaterialRef);
			renderer.DrawIndexed(
				comp.m_MeshRef->GetVertexBuffer(),
				comp.m_MeshRef->GetIndexBuffer(),
				comp.m_MeshRef->GetNumIndices());
		}

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
