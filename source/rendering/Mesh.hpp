#pragma once

#include <PCH.hpp>
#include "Buffer.hpp"
#include "Vertex.hpp"
#include <core/ResourceFwd.hpp>

namespace brk::rdr {
	class BRK_RENDERING_API Mesh : public Resource
	{
	public:
		using Resource::Resource;

		Mesh(
			const Vertex3d* vertices,
			const uint32 nVertices,
			const uint32* indices,
			const uint32 nIndices,
			const ULID& id = ULID::Generate(),
			std::string name = "");

		template <uint32 N1, uint32 N2>
		Mesh(
			const Vertex3d (&vertices)[N1],
			const uint32 (&indices)[N2],
			const ULID& id = ULID::Generate(),
			std::string name = "")
			: Resource(id, std::move(name), {})
			, m_VertexBuffer{ Buffer::VertexBuffer, vertices }
			, m_IndexBuffer{ Buffer::IndexBuffer, indices }
			, m_NumIndices{ N2 }
		{}

		ResourceTypeInfo& GetTypeInfo() const override;

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

		[[nodiscard]] uint32 GetNumIndices() const noexcept { return m_NumIndices; }

		bool DoLoad() override;
		void DoUnload() override;

		~Mesh() = default;

	private:
		friend class MeshWidget;

		Buffer m_VertexBuffer, m_IndexBuffer;
		uint32 m_NumIndices = 0;
	};

	class BRK_RENDERING_API MeshWidget : public ResourceUiWidget
	{
	public:
		MeshWidget() = default;
		void Init(const Resource& res) override;
		bool CreationUi() override;
		bool EditionUi(const Resource& res, bool& out_shouldReload) override;
	};
} // namespace brk::rdr

namespace brk {
	RES_INFO_IMPL(rdr::Mesh, BRK_RENDERING_API);
}