#include "InputSystem.hpp"

#include <entt/entity/registry.hpp>

#include <systems/WindowSystem.hpp>

namespace brk::inputs {
	void System::Update(World& world, const TimeInfo& timeInfo)
	{
		ProcessEvents(world);
	}

	void brk::inputs::System::ProcessEvents(World& world)
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
				break;
			}
			case EEventType::MouseButton:
			{
				const auto& data =
					eventComponent.m_Event.GetData<EEventType::MouseButton>();
				break;
			}
			default: break;
			}
		}
	}

} // namespace brk::inputs
