#include "Texture.hpp"
#include <core/Loaders.hpp>
#include <core/LogManager.hpp>
#include <core/ResourceFormatter.hpp>

#ifdef BRK_EDITOR
#include <imgui.h>
#include <imgui/MiscWidgets.hpp>
#include <SDL3/SDL_dialog.h>
#endif

#include <cstdio>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x) BRK_ASSERT(x, "stbi error")
#include <stb_image.h>

namespace {
	constexpr int32 s_NumChannels[] = {
		1,
		4,
	};

	constexpr SDL_DialogFileFilter s_ImageFileFilters[] = {
		{ "Image Files", { "png;jpg;jpeg;bmp;gif;hdr" } },
	};

	constexpr const char* s_PixelFormatNames[] = { "Grayscale", "RGBA 8" };

	constexpr const char* s_FilterModeNames[] = { "Nearest", "Linear" };

	constexpr const char* s_AddressModeNames[] = { "Clamp", "Wrap", "Mirror", "Border" };

} // namespace

const brk::ResourceTypeInfo brk::rdr::Texture2d::Info =
	brk::ResourceTypeInfo::Create<rdr::Texture2d, rdr::Texture2dWidget>("texture2d");

brk::rdr::Texture2d::Texture2d(const Texture2dSettings& settings, const void* data)
	: Resource(ULID::Generate())
	, m_Settings{ settings }
{
	Init(data);
}

brk::rdr::Texture2d::Texture2d(
	const Texture2dSettings& settings,
	const ULID& id,
	std::string name,
	std::string imagePath)
	: Resource(id, std::move(name), std::move(imagePath))
	, m_Settings{ settings }
{}

bool brk::rdr::Texture2d::DoLoad()
{
	if (m_Handle.m_Tex)
		return true;

	BRK_ASSERT(
		m_FilePath.length(),
		"Can't load texture {}: empty file path",
		static_cast<const Resource&>(*this));
	int32 width = 0, height = 0, numChannels = 0;
	FILE* file = std::fopen(m_FilePath.c_str(), "rb");
	if (!file)
	{
		BRK_LOG_ERROR(
			"Cannot load texture {} from file {}: {}",
			static_cast<const Resource&>(*this),
			m_FilePath,
			std::strerror(errno));
		return false;
	}

	stbi_uc* data = stbi_load_from_file(
		file,
		&width,
		&height,
		&numChannels,
		s_NumChannels[ToUnderlying(m_Settings.m_Format)]);

	fclose(file);

	if (!data)
	{
		BRK_LOG_ERROR(
			"Failed to load texture content from {}: {}",
			m_FilePath,
			stbi_failure_reason());
		return false;
	}
	m_Settings.m_Width = uint32(width);
	m_Settings.m_Height = uint32(height);
	Init(data);
	stbi_image_free(data);

	return true;
}

void brk::rdr::Texture2d::DoUnload()
{
	if (!m_FilePath.empty())
		Reset();
}

void brk::rdr::Texture2dWidget::Init(const Resource& res)
{
	const auto& tex = static_cast<const Texture2d&>(res);
	m_Id = tex.m_Id;
	m_Name = tex.m_Name;
	m_FilePath = tex.m_FilePath;
	m_Settings = tex.m_Settings;
}

bool brk::rdr::Texture2dWidget::CreationUi()
{
	dev_ui::ULIDWidget("ULID", m_Id);
	dev_ui::StdStringInput("Texture Name", m_Name);
	dev_ui::FilePathInput("Image File", m_FilePath, false, s_ImageFileFilters, 1);

	dev_ui::EnumDropDown(
		"Pixel Format",
		s_PixelFormatNames,
		ArraySize(s_PixelFormatNames),
		m_Settings.m_Format);
	dev_ui::EnumDropDown(
		"Filter Mode",
		s_FilterModeNames,
		ArraySize(s_FilterModeNames),
		m_Settings.m_FilterMode);
	dev_ui::EnumDropDown(
		"UV Address Mode",
		s_AddressModeNames,
		ArraySize(s_AddressModeNames),
		m_Settings.m_UvAddressMode);
	if (m_Settings.m_UvAddressMode == EAddressMode::Border)
	{
		ImGui::ColorEdit4("Border Color", &m_Settings.m_BorderColor.x);
	}

	dev_ui::FlagCheckbox(
		"Is Shader Resource",
		m_Settings.m_Options,
		ETextureOptions::ShaderResource);
	ImGui::SameLine();
	dev_ui::FlagCheckbox(
		"Is Render Target",
		m_Settings.m_Options,
		ETextureOptions::RenderTarget);

	dev_ui::FlagCheckbox("Is Dynamic", m_Settings.m_Options, ETextureOptions::Dynamic);

	return m_Name.length() && m_FilePath.length();
}

void brk::rdr::Texture2dWidget::Commit(Resource& out_res) const
{
	auto& tex = static_cast<Texture2d&>(out_res);
	tex.m_Name = m_Name;
	tex.m_FilePath = m_FilePath;
	tex.m_Settings = m_Settings;
}

inline bool brk::JsonLoader<brk::rdr::Texture2d>::Load(
	rdr::Texture2d& out_tex,
	const nlohmann::json& json)
{
	if (out_tex.m_FilePath.empty())
	{
		BRK_LOG_WARNING(
			"Texture {} has empty file path",
			static_cast<const Resource&>(out_tex));
		return false;
	}

	using rdr::EPixelFormat, rdr::EAddressMode, rdr::EFilterMode, rdr::ETextureOptions;
	EPixelFormat format = EPixelFormat::RGBA8;
	Visit("format", json, format);
	DEBUG_CHECK(format > EPixelFormat::Invalid && format < EPixelFormat::NumFormats)
	{
		BRK_LOG_WARNING(
			"Texture {} has invalid pixel format: {}",
			static_cast<const Resource&>(out_tex),
			int32(format));
		format = EPixelFormat::RGBA8;
	}
	EAddressMode addressMode = EAddressMode::Wrap;
	Visit("addressMode", json, addressMode);
	DEBUG_CHECK(
		addressMode > EAddressMode::Invalid && addressMode < EAddressMode::NumModes)
	{
		BRK_LOG_WARNING(
			"Texture {} has invalid address mode: {}",
			static_cast<const Resource&>(out_tex),
			int32(addressMode));
		addressMode = EAddressMode::Wrap;
	}

	EFilterMode filterMode = EFilterMode::Nearest;
	Visit("filterMode", json, filterMode);
	DEBUG_CHECK(filterMode > EFilterMode::Invalid && filterMode < EFilterMode::NumModes)
	{
		BRK_LOG_WARNING(
			"Texture {} has invalid filter mode: {}",
			static_cast<const Resource&>(out_tex),
			int32(filterMode));
		filterMode = EFilterMode::Nearest;
	}
	bool shaderResource = true, renderTarget = false, dynamic = false;
	Visit("isShaderResource", json, shaderResource);
	Visit("isRenderTarget", json, renderTarget);
	Visit("isDynamic", json, dynamic);
	DEBUG_CHECK(shaderResource || renderTarget)
	{
		BRK_LOG_WARNING(
			"Texture {} doesn't have either option ShaderResource or "
			"RenderTarget set. This is invalid.",
			static_cast<const Resource&>(out_tex));
		shaderResource = true;
	}
	out_tex.m_Settings.m_Format = static_cast<EPixelFormat>(format);
	out_tex.m_Settings.m_FilterMode = static_cast<EFilterMode>(filterMode);
	out_tex.m_Settings.m_UvAddressMode = static_cast<EAddressMode>(addressMode);

	if (shaderResource)
		out_tex.m_Settings.m_Options.Set(ETextureOptions::ShaderResource);
	if (renderTarget)
		out_tex.m_Settings.m_Options.Set(ETextureOptions::RenderTarget);
	if (dynamic)
		out_tex.m_Settings.m_Options.Set(ETextureOptions::Dynamic);

	return true;
}

void brk::JsonLoader<brk::rdr::Texture2d>::Save(
	const rdr::Texture2d& tex,
	nlohmann::json& out_json)
{
	using brk::rdr::ETextureOptions;

	out_json["format"] = tex.m_Settings.m_Format;
	out_json["addressMode"] = tex.m_Settings.m_UvAddressMode;
	out_json["filterMode"] = tex.m_Settings.m_FilterMode;
	if (tex.m_Settings.m_Options.HasAny(ETextureOptions::ShaderResource))
		out_json["isShaderResource"] = true;
	if (tex.m_Settings.m_Options.HasAny(ETextureOptions::RenderTarget))
		out_json["isRenderTarget"] = true;
	if (tex.m_Settings.m_Options.HasAny(ETextureOptions::Dynamic))
		out_json["isDynamic"] = true;
}