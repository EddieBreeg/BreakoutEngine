#pragma once

#include <PCH.hpp>

namespace breakout {
	template <auto>
	struct EventData;

	template <auto...>
	class Event;
} // namespace breakout

namespace breakout::inputs {
	enum EEventType : uint8
	{
		MouseMove,
		MouseButton,
		Key,
	};

} // namespace breakout::inputs

namespace breakout {
	template <>
	struct EventData<inputs::EEventType::MouseMove>
	{
		float2 m_OldPos, m_NewPos;
	};

	template <>
	struct EventData<inputs::EEventType::MouseButton>
	{
		uint8 m_Button;
		bool m_Pressed;
		uint8 m_Clicks;
		float2 m_Pos;
	};

	template <>
	struct EventData<inputs::EEventType::Key>
	{
		int32 m_Key;
		uint16 m_Modifiers;
		uint8 m_Pressed;
		uint8 m_Repeat;
	};

	using TInputEvent = Event<inputs::MouseMove, inputs::MouseButton, inputs::Key>;
} // namespace breakout
