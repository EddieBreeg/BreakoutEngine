#include <rendering/Buffer.hpp>
#include <core/Assert.hpp>
#include <core/LogManager.hpp>

#include <rendering/Renderer.hpp>
#include "Renderer.hpp"

#include <d3d11.h>
#include <comdef.h>

namespace {
	constexpr D3D11_BIND_FLAG s_BindFlags[] = {
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_BIND_INDEX_BUFFER,
		D3D11_BIND_CONSTANT_BUFFER,
	};
}

namespace brk::rdr {
	Buffer::Buffer(
		EType type,
		uint32 size,
		const void* data,
		EnumFlags<EBufferOptions> options)
		: m_Type{ type }
		, m_Options{ options }
	{
		BRK_ASSERT(
			type > EType::Invalid && type < EType::NumTypes,
			"Invalid buffer type");
		CD3D11_BUFFER_DESC desc{
			size,
			UINT(s_BindFlags[type]),
			D3D11_USAGE_DEFAULT,
		};
		BRK_ASSERT(
			!options.HasAll(EBufferOptions::CpuRead | EBufferOptions::Dynamic),
			"Buffer options CpuRead and Dynamic are mutually exclusive");

		if (options.HasAny(EBufferOptions::CpuRead))
			desc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;

		if (options.HasAny(EBufferOptions::Dynamic))
		{
			m_Options.Set(EBufferOptions::CpuWrite);
			desc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
			desc.Usage = D3D11_USAGE_DYNAMIC;
		}

		m_Handle = Renderer::s_Instance.GetData()->CreateBuffer(desc, data);
	}

	void brk::rdr::Buffer::SetData(const void* data, uint32 size)
	{
		BRK_ASSERT(
			m_Options.HasAny(EBufferOptions::Dynamic | EBufferOptions::CpuWrite),
			"Called set data on buffer which doesn't have write access");
		ID3D11DeviceContext* ctx = Renderer::s_Instance.GetData()->m_DeviceContext;
		if (!m_Options.HasAny(EBufferOptions::Dynamic))
		{
			ctx->UpdateSubresource(m_Handle, 0, nullptr, data, size, size);
			return;
		}
		D3D11_MAPPED_SUBRESOURCE map;
		const HRESULT err = ctx->Map(m_Handle, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		if (err)
		{
			BRK_LOG_ERROR("Failed to map buffer: {}", _com_error(err).ErrorMessage());
			return;
		}
		std::memcpy(map.pData, data, size);
		ctx->Unmap(m_Handle, 0);
	}

	Buffer::~Buffer() = default;

} // namespace brk::rdr
