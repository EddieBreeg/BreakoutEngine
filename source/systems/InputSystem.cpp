#include "InputSystem.hpp"

#include <entt/entity/registry.hpp>

#include <fmt/core.h>

#include <systems/WindowSystem.hpp>

namespace bre::inputs {
	void System::Update(World& world, const TimeInfo& timeInfo)
	{
		ProcessEvents(world);
	}

	void bre::inputs::System::ProcessEvents(World& world)
	{
		using TQuery = ecs::query::Include<const inputs::EventOneFrameComponent>;
		ecs::QueryWorld<TQuery> queryResults = world.Query<TQuery>();
		for (entt::entity event : queryResults)
		{
			const auto& eventComponent =
				queryResults.Get<const inputs::EventOneFrameComponent>(event);
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

} // namespace bre::inputs
