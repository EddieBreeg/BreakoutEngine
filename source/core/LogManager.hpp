#pragma once

#include <PCH.hpp>

#include <fmt/base.h>

#include <string>
#include <string_view>

namespace bre {
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

		[[nodiscard]] static LogManager& GetInstance() noexcept
		{
			return s_Instance;
		}

		void Log(LogLevel level, std::string_view message);

		template <class... Args>
		void Log(LogLevel level, fmt::format_string<Args...> fmt, Args&&... args)
		{
			Log(level, fmt::format(fmt, std::forward<Args>(args)...));
		}

	private:
		LogManager() = default;

		static LogManager s_Instance;
	};
} // namespace bre
