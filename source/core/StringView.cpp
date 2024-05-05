#include "StringView.hpp"
#include <nlohmann/json.hpp>

void brk::from_json(const nlohmann::json& json, StringView& str)
{
	std::string out;
	json.get_to(out);
	str = StringView{ out.c_str(), (uint32)out.length() };
}

void brk::to_json(nlohmann::json& json, const StringView& str)
{
	json = std::string_view{ str.GetPtr(), str.GetLen() };
}
