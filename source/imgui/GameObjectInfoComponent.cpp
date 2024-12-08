#include "GameObjectInfoComponent.hpp"

#ifdef BRK_DEV
#include <imgui.h>

bool brk::dbg::GameObjectInfoWidget(GameObjectInfoComponent& comp)
{
	char idStr[27];
	idStr[26] = 0;
	comp.m_Id.ToChars(idStr);
	ImGui::InputText("ULID", idStr, sizeof(idStr), ImGuiInputTextFlags_ReadOnly);
	ImGui::InputText(
		"Name",
		comp.m_Name.data(),
		comp.m_Name.length(),
		ImGuiInputTextFlags_ReadOnly);
	return false;
}
#endif
