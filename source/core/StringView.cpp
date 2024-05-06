#include "StringView.hpp"
#include <nlohmann/json.hpp>

void brk::from_json(const nlohmann::json& json, StringView& str)
{
	std::string_view out;
	json.get_to(out);
	str = StringView{ out.data(), (uint32)out.length() };
}

void brk::to_json(nlohmann::json& json, const StringView& str)
{
	json = std::string_view{ str.GetPtr(), str.GetLen() };
}
