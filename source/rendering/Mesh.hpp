#pragma once

#include <PCH.hpp>
#include "Buffer.hpp"
#include "Vertex.hpp"
#include <core/Resource.hpp>

namespace brk::rdr {
	class Mesh : public Resource
	{
	public:
		using Resource::Resource;

		Mesh(
			const Vertex3d* vertices,
			const uint32 nVertices,
			const uint32* indices,
			const uint32 nIndices);

		template <uint32 N>
		Mesh(const Vertex3d (&vertices)[N], const uint32 (&indices)[N])
			: m_VertexBuffer{ Buffer::VertexBuffer, vertices }
			, m_IndexBuffer{ Buffer::IndexBuffer, indices }
		{}

		[[nodiscard]] Buffer& GetVertexBuffer() noexcept { return m_VertexBuffer; }
		[[nodiscard]] const Buffer& GetVertexBuffer() const noexcept
		{
			return m_VertexBuffer;
		}
		[[nodiscard]] Buffer& GetIndexBuffer() noexcept { return m_IndexBuffer; }
		[[nodiscard]] const Buffer& GetIndexBuffer() const noexcept
		{
			return m_IndexBuffer;
		}

		~Mesh() = default;

	private:
		Buffer m_VertexBuffer, m_IndexBuffer;
	};
} // namespace brk::rdr
