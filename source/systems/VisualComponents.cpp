#include "VisualComponents.hpp"
#include <core/LogManager.hpp>
#include <ecs/Component.hpp>
#include <editor/brk_editor_export.h>
#include <imgui/MiscWidgets.hpp>
#include <managers/ResourceManager.hpp>

template <>
struct brk::ecs::ComponentUiWidget<brk::MeshComponent>
{
	ComponentUiWidget(const MeshComponent&) {}
	bool operator()(MeshComponent& mesh)
	{
		dev_ui::ULIDWidget("Mesh", mesh.m_MeshRef ? mesh.m_MeshRef->GetId() : ULID{}, 1);
		dev_ui::ULIDWidget(
			"Material Instance",
			mesh.m_MaterialRef ? mesh.m_MaterialRef->GetId() : ULID{},
			2);
		return false;
	}
};

namespace brk {
	const ecs::ComponentInfo MeshComponent::Info =
		ecs::ComponentInfo::Create<MeshComponent>("mesh");
} // namespace brk
