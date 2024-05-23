#include "Resource.hpp"
#include "Loaders.hpp"

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
	result &= Visit("file", json, out_res.m_FilePath);
	return result;
}

brk::Resource::~Resource()
{
	DoUnload();
}
