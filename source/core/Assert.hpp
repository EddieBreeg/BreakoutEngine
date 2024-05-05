#pragma once

#include <PCH.hpp>

#ifdef BRK_DEV
#include <fmt/core.h>

namespace brk::_internal {
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
} // namespace brk::_internal

#define BRK_ASSERT(expr, fmt, ...)                                                       \
	brk::_internal::Assert(!!(expr), __FILE__, __LINE__, fmt, __VA_ARGS__)
#else
#define BRK_ASSERT(expr, fmt, ...)
#endif