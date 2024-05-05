#include <core/LogManager.hpp>

#include <math/Vector.hpp>

#include <iostream>

namespace
{
	std::ostream& operator<<(std::ostream& os, const brk::StringView str)
	{
		os.write(str.GetPtr(), str.GetLen());
		return os;
	}
} // namespace


namespace brk {
	LogManager LogManager::s_Instance;

	void LogManager::Log(LogLevel level, const StringView message)
	{
		if (level < m_Level)
			return;

		if (level < LogLevel::Critical)
			std::cout << message << '\n';
		else
			std::cerr << message << '\n';
	}

} // namespace brk
