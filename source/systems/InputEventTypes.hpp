#pragma once

#include <PCH.hpp>

namespace breakout {
	template <auto>
	struct EventData;

	template <auto...>
	class Event;
} // namespace breakout

namespace breakout::inputs {
	enum class EEventType : uint8_t
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
		uint8_t m_Button;
		bool m_Pressed;
		uint8_t m_Clicks;
		float2 m_Pos;
	};

	template <>
	struct EventData<inputs::EEventType::Key>
	{
		int32_t m_Key;
		uint16_t m_Modifiers;
		uint8_t m_Pressed;
		uint8_t m_Repeat;
	};

	using TInputEvent = Event<inputs::EEventType::MouseMove,
							  inputs::EEventType::MouseButton,
							  inputs::EEventType::Key>;
} // namespace breakout
