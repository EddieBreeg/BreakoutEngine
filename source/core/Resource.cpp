#include "Resource.hpp"
#include "Loaders.hpp"
#include "LogManager.hpp"
#include "ULIDFormatter.hpp"
#include <fstream>

brk::Resource::Resource(const ULID id)
	: m_Id{ id }
{}

brk::Resource::Resource(const ULID id, std::string&& name, std::string&& file)
	: m_Id{ id }
	, m_Name{ std::move(name) }
	, m_FilePath{ std::move(file) }
{}

bool brk::JsonLoader<brk::Resource>::Load(Resource& out_res, const nlohmann::json& json)
{
	bool result = true;
	result &= Visit("name", json, out_res.m_Name);
	/* The file path may not always be required */
	Visit("file", json, out_res.m_FilePath);
	return result;
}

brk::Resource::~Resource() = default;

brk::InputByteStream brk::Resource::LoadFileContents()
{
	BRK_ASSERT(
		m_FilePath.length(),
		"Called LoadFileContents on resource {} ({}) with empty file path",
		m_Name,
		m_Id);
	std::ifstream file{ m_FilePath, std::ios_base::binary | std::ios_base::ate };
	DEBUG_CHECK(file.is_open())
	{
		BRK_LOG_ERROR(
			"Failed to load resource file {}: {}",
			m_FilePath,
			std::strerror(errno));
		return {};
	}
	std::vector<char> buf(file.tellg());
	file.read(buf.data(), buf.size());

	DEBUG_CHECK(!(file.eof() || file.fail() || file.bad()))
	{
		BRK_LOG_ERROR(
			"Failed to load resource file {}: {}",
			m_FilePath,
			std::strerror(errno));
		return {};
	}
	return { std::move(buf) };
}
