#pragma once

#include <PCH.hpp>

#ifdef BRK_DEV

namespace brk::dbg
{
	struct Overlay
	{
		bool m_Enabled = false;

		void Draw();

		static Overlay s_Instance;

	private:
		Overlay() = default;
	};
	
} // namespace brk::dbg


#endif
