#include "Resource.hpp"
#include <nlohmann/json.hpp>

namespace {
	template <class T>
	bool Visit(T& out, const char* key, const nlohmann::json& json)
	{
		const auto it = json.find(key);
		if (it == json.end())
			return false;

		it->get_to(out);
		return true;
	}
} // namespace

bool brk::JsonLoader<brk::Resource>::Load(Resource& out_res, const nlohmann::json& json)
{
	bool result = true;
	result &= Visit(out_res.m_Id, "id", json);
	result &= Visit(out_res.m_Name, "name", json);
	result &= Visit(out_res.m_FilePath, "file", json);
	return result;
}

brk::Resource::~Resource()
{
	DoUnload();
	m_IsLoaded = false;
}
