#include "ULID.hpp"
#include "Assert.hpp"
#include "LogManager.hpp"
#include "RNG.hpp"
#include <nlohmann/json.hpp>
#include <ctime>

namespace {
	thread_local brk::RNG s_RNG;
}

brk::ULID brk::ULID::Generate()
{
	std::timespec ts;
	std::timespec_get(&ts, TIME_UTC);
	ULID res;
	res.m_Left = static_cast<uint64>(ts.tv_sec) * 1000 + ts.tv_nsec / 1000000;
	res.m_Left = (res.m_Left << 16) | (s_RNG() & 0xffff);
	res.m_Right = s_RNG();

	return res;
}

void brk::from_json(const nlohmann::json& json, ULID& id)
{
	StringView str;
	DEBUG_CHECK(json.is_string())
	{
		BRK_LOG_WARNING("Failed to convert JSON to ULID (type is {})", json.type_name());
		id = {};
		return;
	}
	json.get_to<StringView>(str);
	id = ULID::FromString(str);
}

void brk::to_json(nlohmann::json& out_json, const ULID& id)
{
	char str[26];
	id.ToChars(str);
	out_json = StringView{ str, 26 };
}
