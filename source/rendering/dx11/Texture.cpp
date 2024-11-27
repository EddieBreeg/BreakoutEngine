#include <rendering/Texture.hpp>
#include <core/DebugBreak.hpp>
#include <rendering/Renderer.hpp>
#include "Renderer.hpp"
#include <d3d11.h>

namespace {
	constexpr DXGI_FORMAT s_Formats[] = {
		DXGI_FORMAT_R8_UINT,
		DXGI_FORMAT_R8G8B8A8_UINT,
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

brk::rdr::Texture2d::Texture2d(const Texture2dSettings& settings, const void* data)
	: Resource(ULID::Generate())
	, m_Settings{ settings }
{
	RendererData& rendererData = *Renderer::s_Instance.GetData();
	BRK_ASSERT(
		settings.m_Format > EPixelFormat::Invalid &&
			settings.m_Format < EPixelFormat::NumFormats,
		"Invalid pixel format: {}",
		int32(settings.m_Format));

	uint32 bindFlags = 0;
	BRK_ASSERT(
		settings.m_Options.HasAny(
			ETextureOptions::RenderTarget | ETextureOptions::ShaderResource),
		"Texture must specify at least the RenderTarget or ShaderResource options");

	uint32 cpuAccess = 0;
	if (settings.m_Options.HasAny(ETextureOptions::ShaderResource))
	{
		bindFlags |= D3D11_BIND_SHADER_RESOURCE;
		cpuAccess |= D3D11_CPU_ACCESS_WRITE;
	}
	if (settings.m_Options.HasAny(ETextureOptions::RenderTarget))
	{
		bindFlags |= D3D11_BIND_RENDER_TARGET;
		cpuAccess |= D3D11_CPU_ACCESS_READ;
	}

	const D3D11_USAGE usage = settings.m_Options.HasAny(ETextureOptions::Dynamic)
								  ? D3D11_USAGE_DYNAMIC
								  : D3D11_USAGE_DEFAULT;
	const CD3D11_TEXTURE2D_DESC desc{
		s_Formats[ToUnderlying(settings.m_Format)],
		settings.m_Width,
		settings.m_Height,
		1,
		0,
		bindFlags,
		usage,
		cpuAccess,
	};
	m_Handle.m_Tex = rendererData.CreateTexture2d(
		desc,
		data,
		s_PixelSizes[ToUnderlying(settings.m_Format)] * settings.m_Width);

	DEBUG_CHECK(m_Handle.m_Tex)
	{
		return;
	}

	if (settings.m_Options.HasAny(ETextureOptions::ShaderResource))
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
		viewDesc.Format = desc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D = { 0, 1 };
		m_Handle.m_ShaderResource =
			rendererData.CreateShaderResourceView(*m_Handle.m_Tex, viewDesc);

		BRK_ASSERT(
			settings.m_FilterMode > EFilterMode::Invalid &&
				settings.m_FilterMode < EFilterMode::NumModes,
			"Invalid filter mode: {}",
			int32(settings.m_FilterMode));
		BRK_ASSERT(
			settings.m_UvAddressMode > EAddressMode::Invalid &&
				settings.m_UvAddressMode < EAddressMode::NumModes,
			"Invalid UV address mode: {}",
			int32(settings.m_UvAddressMode));

		CD3D11_SAMPLER_DESC samplerDesc{ D3D11_DEFAULT };
		samplerDesc.Filter = s_Filters[ToUnderlying(settings.m_FilterMode)];
		samplerDesc.AddressU = s_AddressModes[ToUnderlying(settings.m_UvAddressMode)];
		samplerDesc.AddressV = s_AddressModes[ToUnderlying(settings.m_UvAddressMode)];
		m_Handle.m_Sampler = rendererData.CreateSamplerState(samplerDesc);
		DEBUG_CHECK(m_Handle.m_Sampler)
		{
			dbg::Break();
		}
	}

	if (settings.m_Options.HasAny(ETextureOptions::RenderTarget))
	{
		D3D11_RENDER_TARGET_VIEW_DESC viewDesc = {};
		viewDesc.Format = desc.Format;
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipSlice = 0;
		m_Handle.m_RenderTarget =
			rendererData.CreateRenderTargetView(*m_Handle.m_Tex, viewDesc);
	}
}

brk::rdr::Texture2d::~Texture2d()
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
}