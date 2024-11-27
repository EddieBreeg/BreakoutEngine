#pragma once

#include <PCH.hpp>
#include <core/EnumFlags.hpp>
#include <core/Resource.hpp>

#ifdef BRK_DX11
#include "dx11/ResourceHandles.hpp"
#endif

namespace brk::rdr {
	class Renderer;

	enum class EPixelFormat : int8
	{
		Invalid = -1,
		Grayscale,
		RGBA8,
		NumFormats
	};

	enum class ETextureOptions : uint8
	{
		None = 0,
		ShaderResource = BIT(0),
		RenderTarget = BIT(1),
		Dynamic = BIT(2),
		All
	};

	enum class EFilterMode: int8
	{
		Invalid = -1,
		Nearest,
		Linear,
		NumModes,
	};

	enum class EAddressMode: int8
	{
		Invalid = -1,
		Clamp,
		Wrap,
		Mirror,
		Border,
		NumModes
	};

	struct Texture2dSettings
	{
		EPixelFormat m_Format = EPixelFormat::RGBA8;
		uint32 m_Width = 0, m_Height = 0;
		EnumFlags<ETextureOptions> m_Options = ETextureOptions::ShaderResource;
		EFilterMode m_FilterMode = EFilterMode::Nearest;
		EAddressMode m_UvAddressMode = EAddressMode::Border;
		float4 m_BorderColor;
	};

	class Texture2d : public Resource
	{
	public:
		using Resource::Resource;
		Texture2d(const Texture2dSettings& settings, const void* data = nullptr);

		using HandleType = ResourceHandle<Texture2d>::Type;

		[[nodiscard]] HandleType GetHandle() const noexcept { return m_Handle; }

		~Texture2d();

	private:
		friend class Renderer;
		HandleType m_Handle = {};
		Texture2dSettings m_Settings = {};
	};
} // namespace brk::rdr
