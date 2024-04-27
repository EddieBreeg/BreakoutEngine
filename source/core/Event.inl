namespace brk {
	template <auto... EventTypes>
	Event<EventTypes...>::Event(Event&& other)
		: m_Type{ other.m_Type }
	{
		for (size_t i = 0; i < sizeof(m_Buf); ++i)
			m_Buf[i] = other.m_Buf[i];
		other.m_Type = static_cast<EEvent>(-1);
	}

	template <auto... EventTypes>
	inline Event<EventTypes...>::~Event()
	{
		constexpr bool trivial =
			(std::is_trivially_destructible_v<EventData<EventTypes>> && ...);
		if constexpr (trivial)
			return;
		if (m_Type != static_cast<EEvent>(-1))
			Destroy<0>();
	}

	template <auto... EventTypes>
	inline Event<EventTypes...>::Event(EEvent type)
		: m_Type{ type }
	{}

	template <auto... EventTypes>
	template <size_t I>
	inline void Event<EventTypes...>::Destroy() const
	{
		if constexpr (I < sizeof...(EventTypes))
		{
			constexpr EEvent e = meta::ValueList<EventTypes...>::template Get<I>;
			if (e == m_Type)
				DestroyAs<EventData<e>>();
			else
				Destroy<I + 1>();
		}
	}
} // namespace brk