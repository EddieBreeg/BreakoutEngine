#pragma once

#include <PCH.hpp>

struct ImGuiContext;

namespace brk::dbg
{
	struct BRK_DEV_UI_API Overlay
	{
		bool m_Enabled = false;

		void Draw();

		static Overlay s_Instance;

	private:
		Overlay() = default;
	};
	
} // namespace brk::dbg
