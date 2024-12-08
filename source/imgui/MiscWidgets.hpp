#pragma once

#include <PCH.hpp>
#ifdef BRK_DEV
#include <string>

namespace brk::dev_ui {
	BRK_DEV_UI_API bool StdStringInput(
		const char* label,
		std::string& out_string,
		int flags = 0);
} // namespace brk::ui

#endif