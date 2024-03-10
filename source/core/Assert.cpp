#include "Assert.hpp"

#include <core/LogManager.hpp>

#ifdef _WIN32
#include <intrin.h>
#define DEBUGBREAK __debugbreak
#else
#include <csignal>
#define DEBUGBREAK() raise(SIGTRAP)
#endif

namespace breakout::_internal {
	void AssertImpl(std::string&& message, const char* file, int line)
	{
		LogManager::GetInstance().Log(LogManager::LogLevel::Critical,
									  "\033[1;31m[ASSERT] {}:{}: {}\033[0m",
									  file,
									  line,
									  std::move(message));
		DEBUGBREAK();
	}

} // namespace breakout::_internal
