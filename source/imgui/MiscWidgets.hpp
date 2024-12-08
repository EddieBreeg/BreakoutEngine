#pragma once

#include <PCH.hpp>
#ifdef BRK_DEV
#include <string>

struct SDL_DialogFileFilter;

namespace brk {
	class ULID;
}

namespace brk::dev_ui {
	BRK_DEV_UI_API bool StdStringInput(
		const char* label,
		std::string& out_string,
		int32 flags = 0);

	BRK_DEV_UI_API bool FilePathInput(
		const char* label,
		std::string& out_path,
		bool saveFile = false,
		const SDL_DialogFileFilter* filters = nullptr,
		int32 nFilters = 0,
		int32 flags = 0,
		int32 stackId = 0);

	BRK_DEV_UI_API void ULIDWidget(
		const char* label,
		const ULID& ulid,
		int32 stackId = 0);

	/**
	 * A convenience widget to toggle one or multiple flags with a single checkbox
	 * \param label: The label to put beside the checkbox
	 * \param flags: The flags to modify
	 * \param switches: The flags which will get flipped whenever the checkbox changes
	 * state
	 * \returns If the checkbox was clicked, returns the modifed flags (that is, flags &
	 * switches). Otherwise, returns flags.
	 */
	BRK_DEV_UI_API uint64 FlagCheckbox(const char* label, uint64 flags, uint64 switches);
} // namespace brk::dev_ui

#endif