#include "InputSystem.hpp"

#include <entt/entity/registry.hpp>

#include <fmt/core.h>

#include <systems/WindowSystem.hpp>

namespace breakout::inputs {
	void System::Update(World& world, const TimeInfo& timeInfo)
	{
		ProcessEvents(world);
	}

	void breakout::inputs::System::ProcessEvents(World& world)
	{
		for (entt::entity event : world)
		{
			const auto& eventComponent =
				world.Get<const inputs::EventOneFrameComponent>(event);
			switch (eventComponent.m_Event.GetType())
			{
			case EEventType::MouseMove:
			{
				const auto& data =
					eventComponent.m_Event.GetData<EEventType::MouseMove>();
				fmt::println("Mouse moved: ({}, {}) -> ({}, {})",
							 data.m_OldPos.x,
							 data.m_OldPos.y,
							 data.m_NewPos.x,
							 data.m_NewPos.y);
				break;
			}
			case EEventType::MouseButton:
			{
				const auto& data =
					eventComponent.m_Event.GetData<EEventType::MouseButton>();
				fmt::println("Mouse button {} at ({}, {})",
							 data.m_Pressed ? "pressed" : "released",
							 data.m_Pos.x,
							 data.m_Pos.y);
				break;
			}
			default: break;
			}
		}
	}

} // namespace breakout::inputs
