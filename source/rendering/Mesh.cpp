#include "Mesh.hpp"

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
