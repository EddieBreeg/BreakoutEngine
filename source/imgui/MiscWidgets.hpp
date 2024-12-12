#pragma once

#include <PCH.hpp>
#ifdef BRK_DEV
#include <core/EnumFlags.hpp>
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

	namespace _internal {
		BRK_DEV_UI_API uint64
		FlagCheckboxImpl(const char* label, uint64 flags, uint64 switches);

		BRK_DEV_UI_API uint32 EnumDropDownImpl(
			const char* label,
			const char* const* valueNames,
			uint32 numValues,
			uint32 currentValue);
	} // namespace _internal

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

	template <class Int, std::enable_if_t<std::is_integral_v<Int>>* = nullptr>
	bool FlagCheckbox(const char* label, Int& out_flags, Int switches)
	{
		const Int newFlags = static_cast<Int>(_internal::FlagCheckboxImpl(
			label,
			static_cast<uint64>(out_flags),
			static_cast<uint64>(switches)));
		const bool ret = (newFlags != out_flags);
		out_flags = newFlags;
		return ret;
	}

	template <class E>
	bool FlagCheckbox(const char* label, EnumFlags<E>& out_flags, E switches)
	{
		const E newFlags = static_cast<E>(_internal::FlagCheckboxImpl(
			label,
			static_cast<uint64>(out_flags.Get()),
			static_cast<uint64>(switches)));
		const bool ret = newFlags != out_flags.Get();
		out_flags = newFlags;
		return ret;
	}

	template <class E, std::enable_if_t<std::is_enum_v<E>>* = nullptr>
	bool EnumDropDown(
		const char* label,
		const char* const* valueNames,
		uint32 numValues,
		E& inout_currentValue,
		E minValue = static_cast<E>(0))
	{
		const E newVal = static_cast<E>(
			_internal::EnumDropDownImpl(
				label,
				valueNames,
				numValues,
				static_cast<uint32>(inout_currentValue)) +
			static_cast<uint32>(minValue));
		const bool ret = newVal != inout_currentValue;
		inout_currentValue = newVal;
		return ret;
	}
} // namespace brk::dev_ui

#endif