#pragma once

#include <PCH.hpp>
#include <core/ResourceRef.hpp>
#include <rendering/Material.hpp>
#include <rendering/Mesh.hpp>

namespace brk {
	/* Holds no data. This component is simply here to indicate an entity is meant to be
	 * rendererd*/
	struct VisualComponent
	{};

	struct MeshComponent
	{
		ResourceRef<rdr::Material> m_MaterialRef;
		ResourceRef<rdr::Mesh> m_MeshRef;
	};

} // namespace brk
