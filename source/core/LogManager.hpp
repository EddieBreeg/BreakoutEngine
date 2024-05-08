#pragma once

#include <PCH.hpp>

#include <string>
#include "StringView.hpp"
#include "StringViewFormatter.hpp"

namespace brk {
	class LogManager
	{
	public:
		enum LogLevel : uint8
		{
			Trace,
			Warning,
			Critical,
		};

		LogManager(const LogManager&) = delete;

		LogLevel m_Level = Critical;

		[[nodiscard]] static LogManager& GetInstance() noexcept { return s_Instance; }

		void Log(LogLevel level, const StringView message);

		template <class... Args>
		void Log(LogLevel level, fmt::format_string<Args...> fmt, Args&&... args)
		{
			const std::string str = fmt::format(fmt, std::forward<Args>(args)...);
			Log(level, StringView{ str.c_str(), (uint32)str.length() });
		}

	private:
		LogManager() = default;

		static LogManager s_Instance;
	};
} // namespace brk

#define BRK_LOG_WARNING(fmt, ...)                                                        \
	brk::LogManager::GetInstance().Log(brk::LogManager::Warning, fmt, __VA_ARGS__)