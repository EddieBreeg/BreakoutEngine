#include "Assert.hpp"
#ifdef BRK_DEBUG

#include <core/LogManager.hpp>

#include "DebugBreak.hpp"

namespace brk::_internal {
	void AssertImpl(std::string&& message, const char* file, int line)
	{
		LogManager::GetInstance().Log(LogManager::LogLevel::Critical,
									  "\033[1;31m[ASSERT] {}({}): {}\033[0m",
									  file,
									  line,
									  std::move(message));
		dbg::Break();
	}
} // namespace brk::_internal

#endif