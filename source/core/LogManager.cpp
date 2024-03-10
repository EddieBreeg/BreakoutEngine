#include <core/LogManager.hpp>

#include <math/Vector.hpp>

#include <iostream>

namespace breakout {
	LogManager LogManager::s_Instance;

	void LogManager::Log(LogLevel level, std::string_view message)
	{
		if (level < m_Level)
			return;

		if (level < LogLevel::Critical)
			std::cout << message << '\n';
		else
			std::cerr << message << '\n';
	}

} // namespace breakout
