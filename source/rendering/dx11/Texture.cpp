#include <rendering/Texture.hpp>
#include <core/DebugBreak.hpp>
#include <rendering/Renderer.hpp>
#include "Renderer.hpp"
#include <d3d11.h>

namespace {
	constexpr DXGI_FORMAT s_Formats[] = {
		DXGI_FORMAT_R8_UNORM,
		DXGI_FORMAT_R8G8B8A8_UNORM,
	};

	constexpr uint32 s_PixelSizes[] = {
		1,
		4,
	};

	constexpr D3D11_FILTER s_Filters[] = {
		D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
	};
	constexpr D3D11_TEXTURE_ADDRESS_MODE s_AddressModes[] = {
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		D3D11_TEXTURE_ADDRESS_BORDER,
	};
} // namespace

void brk::rdr::Texture2d::Init(const void* data)
{
	RendererData& rendererData = *Renderer::s_Instance.GetData();
	BRK_ASSERT(
		m_Settings.m_Format > EPixelFormat::Invalid &&
			m_Settings.m_Format < EPixelFormat::NumFormats,
		"Invalid pixel format: {}",
		int32(m_Settings.m_Format));

	uint32 bindFlags = 0;
	BRK_ASSERT(
		m_Settings.m_Options.HasAny(
			ETextureOptions::RenderTarget | ETextureOptions::ShaderResource),
		"Texture must specify at least the RenderTarget or ShaderResource options");

	uint32 cpuAccess = 0;
	if (m_Settings.m_Options.HasAny(ETextureOptions::ShaderResource))
	{
		bindFlags |= D3D11_BIND_SHADER_RESOURCE;
		cpuAccess |= D3D11_CPU_ACCESS_WRITE;
	}
	if (m_Settings.m_Options.HasAny(ETextureOptions::RenderTarget))
	{
		bindFlags |= D3D11_BIND_RENDER_TARGET;
		cpuAccess |= D3D11_CPU_ACCESS_READ;
	}

	const D3D11_USAGE usage = m_Settings.m_Options.HasAny(ETextureOptions::Dynamic)
								  ? D3D11_USAGE_DYNAMIC
								  : D3D11_USAGE_DEFAULT;
	const CD3D11_TEXTURE2D_DESC desc{
		s_Formats[ToUnderlying(m_Settings.m_Format)],
		m_Settings.m_Width,
		m_Settings.m_Height,
		1,
		1,
		bindFlags,
		usage,
		cpuAccess,
	};
	m_Handle.m_Tex = rendererData.CreateTexture2d(
		desc,
		data,
		s_PixelSizes[ToUnderlying(m_Settings.m_Format)] * m_Settings.m_Width);

	DEBUG_CHECK(m_Handle.m_Tex)
	{
		return;
	}

	if (m_Settings.m_Options.HasAny(ETextureOptions::ShaderResource))
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format = desc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D = { 0, 1 };
		m_Handle.m_ShaderResource =
			rendererData.CreateShaderResourceView(*m_Handle.m_Tex, viewDesc);

		BRK_ASSERT(
			m_Settings.m_FilterMode > EFilterMode::Invalid &&
				m_Settings.m_FilterMode < EFilterMode::NumModes,
			"Invalid filter mode: {}",
			int32(m_Settings.m_FilterMode));
		BRK_ASSERT(
			m_Settings.m_UvAddressMode > EAddressMode::Invalid &&
				m_Settings.m_UvAddressMode < EAddressMode::NumModes,
			"Invalid UV address mode: {}",
			int32(m_Settings.m_UvAddressMode));

		CD3D11_SAMPLER_DESC samplerDesc{ D3D11_DEFAULT };
		samplerDesc.Filter = s_Filters[ToUnderlying(m_Settings.m_FilterMode)];
		samplerDesc.AddressU = s_AddressModes[ToUnderlying(m_Settings.m_UvAddressMode)];
		samplerDesc.AddressV = s_AddressModes[ToUnderlying(m_Settings.m_UvAddressMode)];
		m_Handle.m_Sampler = rendererData.CreateSamplerState(samplerDesc);
		DEBUG_CHECK(m_Handle.m_Sampler)
		{
			dbg::Break();
		}
	}

	if (m_Settings.m_Options.HasAny(ETextureOptions::RenderTarget))
	{
		D3D11_RENDER_TARGET_VIEW_DESC viewDesc = {};
		viewDesc.Format = desc.Format;
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipSlice = 0;
		m_Handle.m_RenderTarget =
			rendererData.CreateRenderTargetView(*m_Handle.m_Tex, viewDesc);
	}
}

void brk::rdr::Texture2d::Reset()
{
	if (!m_Handle.m_Tex)
		return;
	m_Handle.m_Tex->Release();
	if (m_Handle.m_ShaderResource)
	{
		m_Handle.m_ShaderResource->Release();
		m_Handle.m_Sampler->Release();
	}
	if (m_Handle.m_RenderTarget)
		m_Handle.m_RenderTarget->Release();
	m_Handle = {};
}

brk::rdr::Texture2d::~Texture2d()
{
	Reset();
}