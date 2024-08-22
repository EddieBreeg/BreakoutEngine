#pragma once

#define IMGUI_LEFT_LABEL(func, label, ...)                                               \
	(ImGui::TextUnformatted(label), ImGui::SameLine(), func("##" label, __VA_ARGS__))