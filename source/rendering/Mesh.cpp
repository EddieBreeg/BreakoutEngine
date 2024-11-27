#include "Mesh.hpp"

brk::rdr::Mesh::Mesh(
	const Vertex3d* vertices,
	const uint32 nVertices,
	const uint32* indices,
	const uint32 nIndices)
	: m_VertexBuffer{ Buffer::VertexBuffer, vertices, nVertices }
	, m_IndexBuffer{ Buffer::IndexBuffer, indices, nIndices }
{}