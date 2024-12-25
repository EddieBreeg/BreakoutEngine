#pragma once

#include <PCH.hpp>

struct ImGuiContext;
struct SDL_Window;

namespace brk::rdr {
	struct RendererData;
} // namespace brk::rdr

namespace brk::dev_ui {
	struct BRK_DEV_UI_API Context
	{
		Context() = default;

		void Init(
			ImGuiContext& imguiContext,
			SDL_Window* window,
			rdr::RendererData& data);
		void Shutdown();
		static Context s_Instance;

		SDL_Window* GetWindow() const noexcept { return m_Window; }

		void FrameStart();
		void FrameEnd();
		void Render();

		void UpdateWindows();

	private:
		ImGuiContext* m_ImGuiContext = nullptr;
		SDL_Window* m_Window = nullptr;
	};
} // namespace brk::dev_ui
