#include "VisualSystem.hpp"

#include "TransformComponent.hpp"
#include "VisualComponents.hpp"

#include <core/Resource.hpp>
#include <entt/entity/registry.hpp>
#include <rendering/Renderer.hpp>

#ifdef BRK_DEV
#include <imgui/DebugOverlay.hpp>
#include <editor/Editor.hpp>
#include <imgui.h>
#endif


namespace brk {
	VisualSystem::VisualSystem() {}

	void VisualSystem::Update(World& world, const TimeInfo&)
	{
		auto& renderer = rdr::Renderer::s_Instance;

		renderer.StartRender();

		auto meshes = world.Query<
			ecs::query::Include<const MeshComponent, const TransformComponent>>();
		for (entt::entity m : meshes)
		{
			const auto& comp = meshes.Get<const MeshComponent>(m);
			if (!comp)
				continue;

			renderer.SetMaterial(*comp.m_MaterialRef);
			const auto& transform = meshes.Get<const TransformComponent>(m);
			renderer.SetModelMatrix(transform.GetMatrix());
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
