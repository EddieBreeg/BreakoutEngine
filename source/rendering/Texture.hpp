#pragma once

#include <PCH.hpp>
#include <core/EnumFlags.hpp>
#include <core/LoadersFwd.hpp>
#include <core/ResourceFwd.hpp>

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

	enum class EFilterMode : int8
	{
		Invalid = -1,
		Nearest,
		Linear,
		NumModes,
	};

	enum class EAddressMode : int8
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

	[[nodiscard]] constexpr bool operator==(
		const Texture2dSettings& left,
		const Texture2dSettings& right) noexcept
	{
		return left.m_Format == right.m_Format && left.m_Width == right.m_Width &&
			   left.m_Height == right.m_Height && left.m_Options == right.m_Options &&
			   left.m_FilterMode == right.m_FilterMode &&
			   left.m_UvAddressMode == right.m_UvAddressMode &&
			   left.m_BorderColor == right.m_BorderColor;
	}

	[[nodiscard]] constexpr bool operator!=(
		const Texture2dSettings& left,
		const Texture2dSettings& right) noexcept
	{
		return !(left == right);
	}

	class Texture2d : public Resource
	{
	public:
		ResourceTypeInfo& GetTypeInfo() const override;

		using Resource::Resource;
		/**
		 * Creates a new texture resource
		 * \param settings The texture's settings
		 * \param data: [optional] If no nullptr, the texture will be created with the
		 * provided image data
		 */
		BRK_RENDERING_API Texture2d(
			const Texture2dSettings& settings,
			const void* data = nullptr);

		/** This initializes the texture's properties, without allocating anything on the
		 * GPU. Use this if you want to pre-add the texture to the resource manager, and
		 * the texture needs to be loaded from a file later
		 * \param settings: The texture's settings
		 * \param id: The resource id
		 * \param name: Texture's name
		 * \param imagePath: Path to the file which will be loaded
		 */
		BRK_RENDERING_API Texture2d(
			const Texture2dSettings& settings,
			const ULID& id,
			std::string name,
			std::string imagePath);

		using HandleType = ResourceHandle<Texture2d>::Type;

		BRK_RENDERING_API bool DoLoad() override;
		BRK_RENDERING_API void DoUnload() override;

		[[nodiscard]] HandleType GetHandle() const noexcept { return m_Handle; }

		BRK_RENDERING_API void Reset();

		BRK_RENDERING_API ~Texture2d();

	private:
		friend struct JsonLoader<Texture2d>;
		friend class Texture2dWidget;

		BRK_RENDERING_API void Init(const void* data = nullptr);

		friend class Renderer;
		HandleType m_Handle = {};
		Texture2dSettings m_Settings = {};
	};
} // namespace brk::rdr

template <>
struct BRK_RENDERING_API brk::JsonLoader<brk::rdr::Texture2d>
{
	static bool Load(rdr::Texture2d& out_tex, const nlohmann::json& json);
	static void Save(const rdr::Texture2d& tex, nlohmann::json& out_json);
};

namespace brk {
	RES_INFO_IMPL(rdr::Texture2d, BRK_RENDERING_API);
}