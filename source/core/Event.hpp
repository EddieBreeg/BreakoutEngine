#pragma once

#include <PCH.hpp>

#include <core/Assert.hpp>
#include <core/MetaLists.hpp>
#include <core/Storage.hpp>

namespace brk {
	/** Stores the relevant data for a specific event type.
	 * \tparam EventType: The type of event. Should be an enum value
	 */
	template <auto EventType>
	struct EventData;

	/**
	 * Event: Stores the data for any one the event types specified in the list
	 * \tparam EventTypes: All possible event types this storage can hold data for
	 * \attention All values in the pack must be enum values belonging to the same enum
	 * type
	 */
	template <auto... EventTypes>
	class Event : private StorageFor<EventData<EventTypes>...>
	{
		using EEvent = std::remove_const_t<
			decltype(meta::ValueList<EventTypes...>::template Get<0>)>;

		static_assert(
			(std::is_same_v<EEvent, decltype(EventTypes)> && ...),
			"Inconsistent event types");
		static_assert(std::is_enum_v<EEvent>);

		static_assert(
			((EventTypes != static_cast<EEvent>(-1)) && ...),
			"Value -1 is reserved");

	public:
		Event(const Event&) = delete;
		Event(Event&&);

		/**
		 * Creates an event object from a specified type and list of arguments
		 * \tparam Type: The type of argument to create
		 * \tparam Args: List of arguments to be passed to the constructor of
		 * EventData<Type>
		 */
		template <EEvent Type, class... Args>
		static Event Create(Args&&... args)
		{
			static_assert(((Type == EventTypes) || ...), "Invalid event type");
			Event evt{ Type };
			new (evt.m_Buf) EventData<Type>{ std::forward<Args>(args)... };
			return evt;
		}
		[[nodiscard]] auto GetType() const noexcept { return m_Type; }

		/**
		 * Retrieves the event's data, cast to the requested type.
		 * \warning This function will assert if the specified event type doesn't match
		 * that which is actually stored in the object
		 */
		template <EEvent Type>
		[[nodiscard]] EventData<Type>& GetData()
		{
			static_assert(((Type == EventTypes) || ...), "Invalid event type");
			BRK_ASSERT(Type == m_Type, "Invalid event data access");
			return GetAs<EventData<Type>>();
		}

		/**
		 * Retrieves the event's data, cast to the requested type.
		 * \warning This function will assert if the specified event type doesn't match
		 * that which is actually stored in the object
		 */
		template <EEvent Type>
		[[nodiscard]] const EventData<Type>& GetData() const
		{
			static_assert(((Type == EventTypes) || ...), "Invalid event type");
			BRK_ASSERT(Type == m_Type, "Invalid event data access");
			return GetAs<EventData<Type>>();
		}

		/**
		 * Destroyes the stored event data
		 */
		~Event();

	private:
		template <size_t I>
		void Destroy() const;

		Event(EEvent type);
		EEvent m_Type;
	};

} // namespace brk

#include "Event.inl"