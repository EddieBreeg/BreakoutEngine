#include "ULID.hpp"
#include "RNG.hpp"
#include <nlohmann/json.hpp>
#include <ctime>

brk::ULID brk::ULID::Generate()
{
	std::timespec ts;
	std::timespec_get(&ts, TIME_UTC);
	ULID res;
	res.m_Left = static_cast<uint64>(ts.tv_sec) * 1000 + ts.tv_nsec / 1000000;
	res.m_Left = (res.m_Left << 16) | (RNG::s_Instance() & 0xffff);
	res.m_Right = RNG::s_Instance();

	return res;
}

void brk::from_json(const nlohmann::json& out_json, ULID& id)
{
	id = ULID::FromString(out_json.get<std::string_view>());
}

void brk::to_json(nlohmann::json& out_json, const ULID& id)
{
	char str[26];
	id.ToChars(str);
	out_json = std::string_view{ str, 26 };
}
