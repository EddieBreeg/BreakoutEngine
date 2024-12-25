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

	BRK_CORE_API const char* StrError(int code);

	class BRK_CORE_API LogManager
	{
	public:
		/** Log Level, which acts as a filter mechanism for log messages */
		enum LogLevel : uint8
		{
			Trace,
			Warning,
			Error,
			Critical,
			LogLevelMax
		};

		LogManager(const LogManager&) = delete;
		/** Current log level. Determines which log messages will be output */
		LogLevel m_Level = Error;

		[[nodiscard]] static LogManager& GetInstance() noexcept { return s_Instance; }

		/**
		 * Logs a message
		 * \param level: The priority level of the message. If level < m_Level, the
		 * message will not be output
		 * \param location: The location in code the message originated from
		 * \param message: The message itself
		 */
		void Log(LogLevel level, const CodeLocation location, const StringView message);

		/**
		 * Logs a message
		 * \param level: The priority level of the message. If level < m_Level, the
		 * message will not be output
		 * \param location: The location in code the message originated from
		 * \param fmt: A string which determines how the arguments will be displayed. See
		 * fmtlib for detail
		 * \param args: The arguments to be formatted in the output message
		 */
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

#ifndef BRK_LOGGING_ENABLED
#define BRK_LOGGING_ENABLED BRK_DEBUG
#endif

#if BRK_LOGGING_ENABLED

#define BRK_LOG(level, ...)                                                              \
	brk::LogManager::GetInstance().Log((level), { __FILE__, __LINE__ }, __VA_ARGS__)

#define BRK_LOG_TRACE(...)	  BRK_LOG(brk::LogManager::Trace, __VA_ARGS__)
#define BRK_LOG_WARNING(...)  BRK_LOG(brk::LogManager::Warning, __VA_ARGS__)
#define BRK_LOG_ERROR(...)	  BRK_LOG(brk::LogManager::Error, __VA_ARGS__)
#define BRK_LOG_CRITICAL(...) BRK_LOG(brk::LogManager::Critical, __VA_ARGS__)

#else

#define BRK_LOG(level, ...)

#define BRK_LOG_TRACE(...)
#define BRK_LOG_WARNING(...)
#define BRK_LOG_ERROR(...)
#define BRK_LOG_CRITICAL(...)
#endif