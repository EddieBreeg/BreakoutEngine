#include "MiscWidgets.hpp"

#ifdef BRK_DEV
#include "DevUiContext.hpp"
#include <core/LogManager.hpp>
#include <core/Resource.hpp>
#include <filesystem>
#include <fstream>
#include <imgui.h>
#include <SDL3/SDL_clipboard.h>
#include <SDL3/SDL_dialog.h>
#include <unordered_map>

namespace {
	int ResizeCallback(ImGuiInputTextCallbackData* data)
	{
		std::string* string = static_cast<std::string*>(data->UserData);
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			string->resize(data->BufTextLen);
			data->Buf = string->data();
		}
		return 0;
	}

	void FileOpenCallback(void* userData, const char* const* fileList, int filter)
	{
		auto& string = *static_cast<std::string*>(userData);
		const char* newPath = nullptr;
		if (!fileList || !(newPath = fileList[0]))
			return;
		using std::filesystem::path;

		const path relPath = std::filesystem::relative(newPath);
		string = relPath.u8string();
	}

	void FileSaveCallback(void* userData, const char* const* fileList, int filter)
	{
		auto& string = *static_cast<std::string*>(userData);
		const char* newPath = nullptr;
		if (!fileList || !(newPath = fileList[0]))
			return;
		using std::filesystem::path;

		const path relPath = std::filesystem::relative(newPath);
		string = relPath.u8string();
		// if the file doesn't exist, create it
		if (!std::filesystem::is_regular_file(relPath))
		{
			std::ofstream file{ relPath };
		}
	}
} // namespace

bool brk::dev_ui::StdStringInput(const char* label, std::string& out_string, int flags)
{
	return ImGui::InputText(
		label,
		out_string.data(),
		out_string.size(),
		flags | ImGuiInputTextFlags_CallbackResize,
		&ResizeCallback,
		&out_string);
}

BRK_DEV_UI_API bool brk::dev_ui::FilePathInput(
	const char* label,
	std::string& path,
	bool saveFile,
	const SDL_DialogFileFilter* filters,
	int32 nFilters,
	int flags,
	int stackId)
{
	bool res = StdStringInput(label, path, flags);

	ImGui::SameLine();
	ImGui::PushID(stackId);
	if (ImGui::Button("Choose File"))
	{
		SDL_Window* window = Context::s_Instance.GetWindow();

		res = true;
		if (saveFile)
		{
			SDL_ShowSaveFileDialog(
				FileSaveCallback,
				&path,
				window,
				filters,
				nFilters,
				nullptr);
		}
		else
		{
			SDL_ShowOpenFileDialog(
				FileOpenCallback,
				&path,
				window,
				filters,
				nFilters,
				nullptr,
				false);
		}
	}
	ImGui::PopID();
	return res;
}

BRK_DEV_UI_API void brk::dev_ui::ULIDWidget(
	const char* label,
	const ULID& ulid,
	int stackId)
{
	char idBuf[27];
	idBuf[26] = 0;

	ulid.ToChars(idBuf);
	ImGui::Text("%s: %.26s", label, idBuf);
	ImGui::SameLine();

	ImGui::PushID(stackId);
	if (ImGui::Button("Copy"))
	{
		SDL_SetClipboardText(idBuf);
	}
	ImGui::PopID();
}

uint64 brk::dev_ui::_internal::FlagCheckboxImpl(
	const char* label,
	uint64 currentFlags,
	uint64 switches)
{
	bool enabled = (currentFlags & switches) == switches;
	if (ImGui::Checkbox(label, &enabled))
	{
		currentFlags ^= switches;
	}
	return currentFlags;
}

uint32 brk::dev_ui::_internal::EnumDropDownImpl(
	const char* label,
	const char* const* valueNames,
	uint32 numValues,
	uint32 currentValue)
{
	BRK_ASSERT(numValues > currentValue, "Current value is out of range");
	if (!ImGui::BeginCombo(label, valueNames[currentValue]))
	{
		return currentValue;
	}

	for (uint32 i = 0; i < numValues; i++)
	{
		bool selected = currentValue == i;
		if (ImGui::Selectable(valueNames[i], &selected))
		{
			ImGui::EndCombo();
			return i;
		}
	}

	ImGui::EndCombo();
	return currentValue;
}

bool brk::dev_ui::ResourceFilterWidget(
	const char* label,
	const TULIDMap<Resource*>& resources,
	ULID& selection,
	ImGuiTextFilter* nameFilter,
	const ResourceTypeInfo* typeFilter)
{
	if (nameFilter)
		nameFilter->Draw();

	if (!ImGui::BeginListBox(label))
		return false;

	bool ret = false;

	for (auto&& [id, resource] : resources)
	{
		if (typeFilter && (&resource->GetTypeInfo() != typeFilter))
			continue;

		const std::string& name = resource->GetName();
		if (nameFilter && !nameFilter->PassFilter(name.c_str()))
			continue;

		bool selected = id == selection;
		if (ImGui::Selectable(name.c_str(), selected))
		{
			ret = !selected;
			selection = id;
			break;
		}
	}

	ImGui::EndListBox();
	return ret;
}
#endif
