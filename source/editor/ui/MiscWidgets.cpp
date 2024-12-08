#include "MiscWidgets.hpp"

#ifdef BRK_EDITOR

#include <imgui.h>

namespace {
	int ResizeCallback(ImGuiInputTextCallbackData* data)
	{
 		std::string* string = static_cast<std::string*>(data->UserData);
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			string->resize(data->BufSize);
			data->Buf = string->data();
		}
		return 0;
	}
} // namespace

bool brk::editor::ui::StdStringInput(
	const char* label,
	std::string& out_string,
	int flags)
{
	return ImGui::InputText(
		label,
		out_string.data(),
		out_string.size(),
		flags | ImGuiInputTextFlags_CallbackResize,
		&ResizeCallback,
		&out_string);
}

#endif