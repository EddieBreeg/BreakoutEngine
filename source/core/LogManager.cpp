#include <core/LogManager.hpp>

#include <math/Vector.hpp>

#include <fmt/ostream.h>
#include <iostream>

namespace {
	std::ostream& operator<<(std::ostream& os, const brk::StringView str)
	{
		os.write(str.GetPtr(), str.GetLen());
		return os;
	}

	constexpr const char* s_Styles[brk::LogManager::LogLevelMax] = {
		"\033[37m",
		"\033[1;38;5;202m",
		"\033[1;31m",
		"\033[1;41;37m",
	};
} // namespace

namespace brk {
	LogManager LogManager::s_Instance;

	void LogManager::Log(
		LogLevel level,
		const CodeLocation location,
		const StringView message)
	{
		if (level < m_Level || level >= LogLevelMax)
			return;

		std::ostream& out = level < LogLevel::Critical ? std::cout : std::cerr;
		fmt::print(
			out,
			"{}({}): {}{}\033[0m\n",
			location.m_File,
			location.m_Line,
			s_Styles[level],
			message);
	}

} // namespace brk
