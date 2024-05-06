namespace brk {
	template <class R, class... Args>
	template <class F>
	typename UniqueFunction<R(Args...)>::VTable UniqueFunction<
		R(Args...)>::VTable::Implement()
	{
		VTable t{ TypeId::Get<F>() };

		t.m_Destroy = [](void* ptr)
		{
			if constexpr (std::is_trivially_destructible_v<F>)
				return;
			else if constexpr (sizeof(F) <= sizeof(UniqueFunction::m_Buf))
				static_cast<F*>(ptr)->~F();
			else
				delete static_cast<F*>(ptr);
		};

		if constexpr (std::is_move_assignable_v<F>)
		{
			t.m_MoveAssign = [](void* left, void* right)
			{
				*static_cast<F*>(left) = std::move(*static_cast<F*>(right));
			};
		}
		t.m_Invoke = [](void* ptr, Args... args)
		{
			return (*static_cast<F*>(ptr))(std::forward<Args>(args)...);
		};
		return t;
	}

	template <class R, class... Args>
	template <class F>
	const typename UniqueFunction<R(Args...)>::VTable
		UniqueFunction<R(Args...)>::VTable::s_Instance =
			UniqueFunction<R(Args...)>::VTable::Implement<F>();

	template <class R, class... Args>
	UniqueFunction<R(Args...)>::UniqueFunction(UniqueFunction&& other)
		: m_Table{ other.m_Table }
	{
		for (uint32 i = 0; i < sizeof(m_Buf); ++i)
			m_Buf[i] = other.m_Buf[i];
		other.m_Table = nullptr;
	}

	template <class R, class... Args>
	UniqueFunction<R(Args...)>::~UniqueFunction()
	{
		Reset();
	}

	template <class R, class... Args>
	UniqueFunction<R(Args...)>& UniqueFunction<R(Args...)>::operator=(
		UniqueFunction&& other) noexcept
	{
		std::swap(m_Buf, other.m_Buf);
		std::swap(m_Table, other.m_Table);
		return *this;
	}

	template <class R, class... Args>
	R UniqueFunction<R(Args...)>::operator()(Args... args)
	{
		BRK_ASSERT(m_Table, "Tried to invoke an empty function object");
		return m_Table->m_Invoke(GetPtr(), std::forward<Args>(args)...);
	}

	template <class R, class... Args>
	void UniqueFunction<R(Args...)>::Reset() noexcept
	{
		if (!m_Table)
			return;
		m_Table->m_Destroy(GetPtr());
		m_Table = nullptr;
	}

	template <class R, class... Args>
	const TypeId& UniqueFunction<R(Args...)>::GetId() const noexcept
	{
		return m_Table ? m_Table->m_Id : TypeId::Get<void>();
	}

	template <class R, class... Args>
	void* brk::UniqueFunction<R(Args...)>::GetPtr() noexcept
	{
		return m_Table->m_Id.m_Size <= sizeof(m_Buf) ? m_Buf : m_Ptr;
	}

} // namespace brk