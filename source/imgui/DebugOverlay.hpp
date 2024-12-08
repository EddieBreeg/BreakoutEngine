#pragma once

#include <PCH.hpp>

#ifdef BRK_DEV

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


#endif
