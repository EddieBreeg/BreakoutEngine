#include "Resource.hpp"
#include "Loaders.hpp"
#include "LogManager.hpp"
#include "ResourceFormatter.hpp"
#include <fstream>
#include "ResourceFwd.hpp"

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

void brk::JsonLoader<brk::Resource>::Save(const Resource& res, nlohmann::json& out_json)
{
	out_json["id"] = res.m_Id;
	out_json["name"] = res.m_Name;
	out_json["type"] = res.GetTypeInfo().m_TypeName;
	if (res.m_FilePath.length())
		out_json["file"] = res.m_FilePath;
}

brk::Resource::~Resource()
{
	DoUnload();
}

void brk::Resource::MarkForDeletion()
{
	if (!m_RefCount)
	{
		GetTypeInfo().DestroyResource(this);
		return;
	}
	SetLoadingState(MarkedForDeletion);
}

brk::DynamicArrayStream brk::Resource::LoadFileContents()
{
	BRK_ASSERT(
		m_FilePath.length(),
		"Called LoadFileContents on resource {} with empty file path",
		*this);
	std::ifstream file{ m_FilePath, std::ios_base::binary | std::ios_base::ate };
	DEBUG_CHECK(file.is_open())
	{
		BRK_LOG_ERROR("Failed to load resource file {}: {}", m_FilePath, StrError(errno));
		return {};
	}
	std::vector<char> buf(file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(buf.data(), buf.size());

	DEBUG_CHECK(!(file.eof() || file.fail() || file.bad()))
	{
		BRK_LOG_ERROR("Failed to load resource file {}: {}", m_FilePath, StrError(errno));
		return {};
	}
	return { std::move(buf) };
}

void brk::ResourceDeleter::operator()(Resource* ptr) const
{
	ptr->MarkForDeletion();
}

brk::ResourceTypeInfo::~ResourceTypeInfo()
{
	delete m_Widget;
}

brk::Resource* brk::ResourceTypeInfo::NewResource(const ULID& id)
{
	return m_Constructor(m_Pool, id);
}

void brk::ResourceTypeInfo::DestroyResource(Resource* res)
{
	res->~Resource();
	m_Pool.Deallocate(res, 1);
}