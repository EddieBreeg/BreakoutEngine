#pragma once

#include <PCH.hpp>
#ifdef BRK_EDITOR
#include <string>

namespace brk::editor::ui {
	bool StdStringInput(
		const char* label,
		std::string& out_string,
		int flags = 0);
} // namespace brk::editor::ui

#endif