#pragma once

#include <PCH.hpp>
#include <core/FieldList.hpp>
#include <core/ResourceRef.hpp>
#include <ecs/ComponentFwd.hpp>
#include <rendering/Material.hpp>
#include <rendering/Mesh.hpp>

namespace brk {
	/* Holds no data. This component is simply here to indicate an entity is meant to be
	 * rendererd*/
	struct VisualComponent
	{};

	struct MeshComponent
	{
		ResourceRef<rdr::MaterialInstance> m_MaterialRef;
		ResourceRef<rdr::Mesh> m_MeshRef;

		[[nodiscard]] operator bool() const noexcept
		{
			return m_MaterialRef && m_MeshRef && m_MaterialRef->IsLoaded() &&
				   m_MeshRef->IsLoaded();
		}

		static constexpr meta::
			FieldList<&MeshComponent::m_MaterialRef, &MeshComponent::m_MeshRef>
				Fields = {
					"materialId",
					"meshId",
				};

		BRK_SYSTEMS_API static const ecs::ComponentInfo Info;
	};
} // namespace brk
