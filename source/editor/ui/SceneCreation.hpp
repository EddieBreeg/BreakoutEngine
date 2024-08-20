#pragma once

#include <PCH.hpp>
#ifdef BRK_EDITOR
#include <string>
#include <core/PlatformMacros.hpp>

namespace brk::editor {
	class SceneCreationWindow
	{
	public:
		static SceneCreationWindow s_Instance;
		void Open() noexcept;
		bool Show();

		[[nodiscard]] bool IsOpen() const noexcept { return m_Show; }
		[[nodiscard]] const char* GetScenePath() const noexcept { return m_FilePath; }

	private:
		SceneCreationWindow() = default;

		bool m_Show = false;
		char m_FilePath[BRK_MAX_PATH] = { 0 };
	};
} // namespace brk::editor

#endif