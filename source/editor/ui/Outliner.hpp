#pragma once

#include <PCH.hpp>
#ifdef BRK_EDITOR
#include <entt/entity/fwd.hpp>

namespace brk::editor {
	class Outliner
	{
	public:
		static Outliner s_Instance;
		~Outliner() = default;

		bool Display();
		void Open() noexcept { m_Show = true; }

	private:
		Outliner() = default;
		entt::entity m_SelectedEntity = entt::entity(-1);
		bool m_Show = false;
	};
} // namespace brk::editor

#endif