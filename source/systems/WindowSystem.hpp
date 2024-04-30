#pragma once

#include <PCH.hpp>

#include <core/Event.hpp>

#include <ecs/System.hpp>
#include <ecs/World.hpp>

#include <entt/entity/fwd.hpp>

#include <systems/InputEventTypes.hpp>

struct SDL_Window;

namespace brk
{

	struct WindowSystemSettings
	{
		const char* m_Title = "Breakout";
		int2 m_Size = { 1280, 720 };
		float4 m_ClearColor = { 0, 0, 0, 1 };
		uint32 m_Flags = 0;
	};

	namespace inputs
	{
		struct EventOneFrameComponent
		{
			TInputEvent m_Event;
		};

	} // namespace inputs

	class WindowSystem
	{
	public:
		using World = ecs::WorldView<inputs::EventOneFrameComponent>;
		BRK_ECS_UPDATE_DECL;

		WindowSystem(const WindowSystemSettings& settings = {});
		~WindowSystem() { Terminate(); }

		void Terminate();

	private:

		void ProcessEvents(World& world);

		WindowSystemSettings m_Settings;
		SDL_Window* m_WinPtr = nullptr;
	};

} // namespace brk
