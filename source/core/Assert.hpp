#pragma once

#include <fmt/core.h>

namespace bre {
	namespace _internal {
		void AssertImpl(std::string&& message, const char* file, int line);

		template <class... Args>
		void Assert(bool cond,
					const char* file,
					int line,
					fmt::format_string<Args...> format,
					Args&&... args)
		{
			if (cond)
				return;

			std::string message = fmt::format(format, std::forward<Args>(args)...);
			AssertImpl(std::move(message), file, line);
		}
	} // namespace _internal
} // namespace bre

#define BREAKOUT_ASSERT(expr, fmt, ...)                                             \
	bre::_internal::Assert(!!(expr), __FILE__, __LINE__, fmt, __VA_ARGS__)