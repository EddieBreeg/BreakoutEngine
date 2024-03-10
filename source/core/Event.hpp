#pragma once

#include <PCH.hpp>
#include <core/Assert.hpp>
#include <core/Storage.hpp>

namespace breakout {
	template <auto EventType>
	struct EventData;

	template <auto... EventTypes>
	class Event : private StorageFor<EventData<EventTypes>...>
	{
		using EEvent = decltype(meta::ValueList<EventTypes...>::template Get<0>());

		static_assert((std::is_same_v<EEvent, decltype(EventTypes)> && ...),
					  "Inconsistent event types");
		static_assert(std::is_enum_v<EEvent>);

		static_assert(((EventTypes != static_cast<EEvent>(-1)) && ...),
					  "Value -1 is reserved");

	public:
		Event(const Event&) = delete;
		Event(Event&&);

		template <EEvent Type, class... Args>
		static Event Create(Args&&... args)
		{
			static_assert(((Type == EventTypes) || ...), "Invalid event type");
			Event evt{ Type };
			new (evt.m_Buf) EventData<Type>{ std::forward<Args>(args)... };
			return evt;
		}
		[[nodiscard]] auto GetType() const noexcept { return m_Type; }

		template <EEvent Type>
		[[nodiscard]] EventData<Type>& GetData()
		{
			static_assert(((Type == EventTypes) || ...), "Invalid event type");
			BREAKOUT_ASSERT(Type == m_Type, "Invalid event data access");
			return GetAs<EventData<Type>>();
		}

		template <EEvent Type>
		[[nodiscard]] const EventData<Type>& GetData() const
		{
			static_assert(((Type == EventTypes) || ...), "Invalid event type");
			BREAKOUT_ASSERT(Type == m_Type, "Invalid event data access");
			return GetAs<EventData<Type>>();
		}

		~Event();

	private:
		template <size_t I>
		void Destroy() const;

		Event(EEvent type);
		EEvent m_Type;
	};

} // namespace breakout

#include "Event.inl"