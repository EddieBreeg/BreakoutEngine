#pragma once

#include <PCH.hpp>

namespace brk {
	template <auto>
	struct EventData;

	template <auto...>
	class Event;
} // namespace brk

namespace brk::inputs {
	enum EEventType : uint8
	{
		MouseMove,
		MouseButton,
		Key,
	};

} // namespace brk::inputs

namespace brk {
	template <>
	struct EventData<inputs::EEventType::MouseMove>
	{
		int2 m_OldPos, m_NewPos;
	};

	template <>
	struct EventData<inputs::EEventType::MouseButton>
	{
		uint8 m_Button;
		bool m_Pressed;
		uint8 m_Clicks;
		int2 m_Pos;
	};

	template <>
	struct EventData<inputs::EEventType::Key>
	{
		uint32 m_Key;
		uint16 m_Modifiers;
		uint8 m_Pressed;
		uint8 m_Repeat;
	};

	using TInputEvent = Event<inputs::MouseMove, inputs::MouseButton, inputs::Key>;
} // namespace brk
