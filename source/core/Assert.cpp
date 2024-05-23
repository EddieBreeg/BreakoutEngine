#include "Assert.hpp"
#ifdef BRK_DEBUG

#include <core/LogManager.hpp>

#include "DebugBreak.hpp"

namespace brk::_internal {
	void AssertImpl(std::string&& message, const char* file, const uint32 line)
	{
		LogManager::GetInstance().Log(
			LogManager::LogLevel::Critical,
			{ file, line },
			"\033[1;31m[ASSERT] {}({}): {}\033[0m",
			std::move(message));
		dbg::Break();
	}
} // namespace brk::_internal

#endif