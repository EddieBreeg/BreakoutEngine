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
			"[ASSERT] {}",
			std::move(message));
		dbg::Break();
	}
} // namespace brk::_internal

#endif