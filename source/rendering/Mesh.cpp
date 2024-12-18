#include "Mesh.hpp"
#include <core/Resource.hpp>
#include <imgui/MiscWidgets.hpp>

namespace brk::rdr {
	void MeshWidget::Init(const Resource& res)
	{
		const auto& mesh = static_cast<const Mesh&>(res);
		m_Id = mesh.m_Id;
		m_Name = mesh.m_Name;
		m_FilePath = mesh.m_FilePath;
	}

	bool MeshWidget::CreationUi()
	{
		dev_ui::ULIDWidget("ULID", m_Id);
		dev_ui::StdStringInput("Mesh Name", m_Name);
		dev_ui::FilePathInput("File", m_FilePath, false);
		return false;
	}

	bool MeshWidget::EditionUi(const Resource& res, bool& out_shouldReload)
	{
		if (!CreationUi())
			return false;
		out_shouldReload = m_FilePath != res.GetFile();
		return out_shouldReload || m_Name != res.GetName();
	}
} // namespace brk::rdr

brk::rdr::Mesh::Mesh(
	const Vertex3d* vertices,
	const uint32 nVertices,
	const uint32* indices,
	const uint32 nIndices,
	const ULID& id,
	std::string name)
	: Resource(id, std::move(name), {})
	, m_VertexBuffer{ Buffer::VertexBuffer, vertices, nVertices }
	, m_IndexBuffer{ Buffer::IndexBuffer, indices, nIndices }
	, m_NumIndices{ nIndices }
{}

bool brk::rdr::Mesh::DoLoad()
{
	return m_VertexBuffer && m_IndexBuffer;
}

void brk::rdr::Mesh::DoUnload()
{
	if (!m_FilePath.empty())
	{
		m_VertexBuffer.Reset();
		m_IndexBuffer.Reset();
		m_NumIndices = 0;
	}
}
