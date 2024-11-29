#include "Texture.hpp"
#include <core/LogManager.hpp>
#include <core/ULIDFormatter.hpp>

#include <cstdio>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x) BRK_ASSERT(x, "stbi error")
#include <stb_image.h>

namespace {
	constexpr int32 s_NumChannels[] = {
		1,
		4,
	};
} // namespace

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
		"Can't load texture {} ({}): empty file path",
		m_Name,
		m_Id);
	int32 width = 0, height = 0, numChannels = 0;
	FILE* file = std::fopen(m_FilePath.c_str(), "rb");
	if (!file)
	{
		BRK_LOG_ERROR(
			"Cannot load texture {} ({}) from file {}: {}",
			m_Name,
			m_Id,
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
