#pragma once

#include <PCH.hpp>
#ifdef BRK_EDITOR
#include <core/ULID.hpp>

namespace brk {
	class SceneManager;
} // namespace brk

namespace brk::editor {
	class BRK_EDITOR_API Outliner
	{
	public:
		static Outliner s_Instance;
		~Outliner() = default;

		bool Display(SceneManager& sceneManager);
		void Open() noexcept { m_Show = true; }

		[[nodiscard]] ULID GetSelectedObjectId() const noexcept
		{
			return m_SelectedObjectId;
		}

	private:
		Outliner() = default;
		ULID m_SelectedObjectId;
		bool m_Show = false;
	};
} // namespace brk::editor

#endif