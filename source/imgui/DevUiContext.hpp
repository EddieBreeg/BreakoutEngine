#pragma once

#include <PCH.hpp>

#ifdef BRK_DEV

struct ImGuiContext;
struct SDL_Window;

namespace brk::dev_ui {
	struct BRK_DEV_UI_API Context
	{
		Context() = default;

		void Init(ImGuiContext& imguiContext, SDL_Window* window);
		static Context s_Instance;

		SDL_Window* GetWindow() const noexcept { return m_Window; }

	private:
		ImGuiContext* m_ImGuiContext = nullptr;
		SDL_Window* m_Window = nullptr;
	};
} // namespace brk::dev_ui

#endif