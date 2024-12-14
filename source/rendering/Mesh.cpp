#include "Mesh.hpp"
#include <imgui/MiscWidgets.hpp>

namespace brk::rdr {
	class MeshWidget : public ResourceUiWidget
	{
		void Init(const Resource& res) override
		{
			const auto& mesh = static_cast<const Mesh&>(res);
			m_Id = mesh.m_Id;
			m_Name = mesh.m_Name;
			m_FilePath = mesh.m_FilePath;
		}

		bool CreationUi() override
		{
			dev_ui::ULIDWidget("ULID", m_Id);
			dev_ui::StdStringInput("Mesh Name", m_Name);
			dev_ui::FilePathInput("File", m_FilePath, false);
			return false;
		}

		bool EditionUi(const Resource& res, bool& out_shouldReload) override
		{
			if (!CreationUi())
				return false;
			out_shouldReload = m_FilePath != res.GetFile();
			return out_shouldReload || m_Name != res.GetName();
		}
	};
} // namespace brk::rdr

const brk::ResourceTypeInfo brk::rdr::Mesh::Info =
	brk::ResourceTypeInfo::Create<brk::rdr::Mesh, brk::rdr::MeshWidget>("mesh");

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
