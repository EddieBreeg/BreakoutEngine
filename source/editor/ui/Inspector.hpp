#pragma once

#include <PCH.hpp>

#ifdef BRK_EDITOR
#include <core/ULID.hpp>
#include <entt/entity/fwd.hpp>

namespace brk {
	class SceneManager;
} // namespace brk

namespace brk::editor {
	class Inspector
	{
	public:
		~Inspector() = default;
		void Open() noexcept { m_Show = true; }
		bool Display(entt::registry& entityWorld, const SceneManager& sceneManager);

		static Inspector s_Instance;

		ULID m_SelectedObjectId;

	private:
		Inspector() = default;

		bool m_Show = false;
	};
} // namespace brk::editor

#endif