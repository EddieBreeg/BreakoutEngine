#include "Init.hpp"
#include <imgui.h>

namespace brk::systems::_internal {
	void SetImGuiContext(ImGuiContext& context)
	{
		ImGui::SetCurrentContext(&context);
	}
} // namespace brk::systems::_internal
