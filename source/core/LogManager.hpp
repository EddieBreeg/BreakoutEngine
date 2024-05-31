#pragma once

#include <PCH.hpp>

#include <string>
#include "StringViewFormatter.hpp"

namespace brk {
	struct CodeLocation
	{
		const char* m_File;
		const uint32 m_Line;
	};

	class LogManager
	{
	public:
		enum LogLevel : uint8
		{
			Trace,
			Warning,
			Critical,
			LogLevelMax,
		};

		LogManager(const LogManager&) = delete;

		LogLevel m_Level = Critical;

		[[nodiscard]] static LogManager& GetInstance() noexcept { return s_Instance; }

		void Log(LogLevel level, const CodeLocation location, const StringView message);

		template <class... Args>
		void Log(
			LogLevel level,
			const CodeLocation location,
			fmt::format_string<Args...> fmt,
			Args&&... args)
		{
			const std::string str = fmt::format(fmt, std::forward<Args>(args)...);
			Log(level, location, StringView{ str.c_str(), (uint32)str.length() });
		}

	private:
		LogManager() = default;

		static LogManager s_Instance;
	};
} // namespace brk

#ifdef BRK_DEV

#define BRK_LOG(level, ...)                                                              \
	brk::LogManager::GetInstance().Log((level), { __FILE__, __LINE__ }, __VA_ARGS__)

#define BRK_LOG_TRACE(...)	  BRK_LOG(brk::LogManager::Trace, __VA_ARGS__)
#define BRK_LOG_WARNING(...)  BRK_LOG(brk::LogManager::Warning, __VA_ARGS__)
#define BRK_LOG_CRITICAL(...) BRK_LOG(brk::LogManager::Critical, __VA_ARGS__)

#else

#define BRK_LOG(level, ...) 0

#define BRK_LOG_TRACE(...)	  0
#define BRK_LOG_WARNING(...)  0
#define BRK_LOG_CRITICAL(...) 0
#endif