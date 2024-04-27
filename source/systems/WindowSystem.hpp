#pragma once

#include <PCH.hpp>

#include <core/Event.hpp>
#include <core/Singleton.hpp>

#include <ecs/System.hpp>
#include <ecs/World.hpp>

#include <entt/entity/fwd.hpp>

#include <systems/InputEventTypes.hpp>

struct SDL_Window;

namespace brk {

	struct WindowSystemSettings
	{
		const char* m_Title = "Breakout";
		int32 m_Width = 1280;
		int32 m_Height = 720;
		uint32 m_Flags = 0;
	};

	namespace inputs {
		struct EventOneFrameComponent
		{
			TInputEvent m_Event;
		};

	} // namespace inputs

	class WindowSystem : public Singleton<WindowSystem>
	{
	public:
		using World = ecs::WorldView<inputs::EventOneFrameComponent>;
		BRK_ECS_UPDATE_DECL;

		void Terminate();
		~WindowSystem() = default;

	private:
		friend class Singleton<WindowSystem>;

		void ProcessEvents(World& world);
		WindowSystem(const WindowSystemSettings& settings = {});

		SDL_Window* m_WinPtr = nullptr;
	};

} // namespace brk
