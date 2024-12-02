#pragma once

#include <PCH.hpp>

#ifdef BRK_EDITOR
#include <core/ULID.hpp>
#include <entt/entity/fwd.hpp>

namespace brk {
	class SceneManager;
} // namespace brk

namespace brk::editor {
	class BRK_EDITOR_API Inspector
	{
	public:
		~Inspector() = default;
		void Open() noexcept { m_Show = true; }
		bool Display(entt::registry& entityWorld, SceneManager& sceneManager);

		static Inspector s_Instance;

		ULID m_SelectedObjectId;

	private:
		Inspector() = default;

		bool m_Show = true;
	};
} // namespace brk::editor

#endif