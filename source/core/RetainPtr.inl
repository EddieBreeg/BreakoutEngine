namespace brk {
	template <class T>
	inline uint32 RetainTraits<T>::GetCount(const T* ptr)
	{
		using TBase = typename _internal::RefCountBase<T>::BaseType;
		return static_cast<const TBase*>(ptr)->m_Count;
	}

	template <class T>
	inline void RetainTraits<T>::Increment(T* ptr)
	{
		using TBase = typename _internal::RefCountBase<T>::BaseType;
		++(static_cast<TBase*>(ptr)->m_Count);
	}

	template <class T>
	inline void RetainTraits<T>::Decrement(T* ptr)
	{
		using TBase = typename _internal::RefCountBase<T>::BaseType;
		if (!--(static_cast<TBase*>(ptr)->m_Count))
			delete ptr;
	}

	template <class T, class Traits>
	inline RetainPtr<T, Traits>::RetainPtr(T* ptr, Adopt_t)
		: m_Ptr{ ptr }
	{}

	template <class T, class Traits>
	inline RetainPtr<T, Traits>::RetainPtr(T* ptr, Retain_t)
		: m_Ptr{ ptr }
	{
		if (m_Ptr)
			Traits::Increment(m_Ptr);
	}

	template <class T, class Traits>
	inline RetainPtr<T, Traits>::RetainPtr(T* ptr)
		: RetainPtr{ ptr, Traits::DefaultAction }
	{}

	template <class T, class Traits>
	inline RetainPtr<T, Traits>::~RetainPtr()
	{
		Reset();
	}

	template <class T, class Traits>
	inline RetainPtr<T, Traits>::RetainPtr(const RetainPtr& other)
		: m_Ptr{ other.m_Ptr }
	{
		if (m_Ptr)
			Traits::Increment(m_Ptr);
	}

	template <class T, class Traits>
	inline RetainPtr<T, Traits>::RetainPtr(RetainPtr&& other) noexcept
		: m_Ptr{ other.m_Ptr }
	{
		other.m_Ptr = nullptr;
	}

	template <class T, class Traits>
	inline RetainPtr<T, Traits>& RetainPtr<T, Traits>::operator=(const RetainPtr& other)
	{
		Reset(other.m_Ptr, s_Retain);
		return *this;
	}

	template <class T, class Traits>
	inline RetainPtr<T, Traits>& RetainPtr<T, Traits>::operator=(
		RetainPtr&& other) noexcept
	{
		std::swap(m_Ptr, other.m_Ptr);
		return *this;
	}

	template <class T, class Traits>
	inline void RetainPtr<T, Traits>::Reset(T* ptr, Adopt_t)
	{
		if (m_Ptr)
			Traits::Decrement(m_Ptr);
		m_Ptr = ptr;
	}

	template <class T, class Traits>
	inline void RetainPtr<T, Traits>::Reset(T* ptr, Retain_t)
	{
		if (m_Ptr)
			Traits::Decrement(m_Ptr);
		if (m_Ptr = ptr)
			Traits::Increment(m_Ptr);
	}

	template <class T, class Traits>
	inline void RetainPtr<T, Traits>::Reset(T* ptr)
	{
		Reset(ptr, Traits::DefaultAction);
	}
	template <class T, class Traits>
	inline auto brk::RetainPtr<T, Traits>::GetCount() const
	{
		using TRes = decltype(Traits::GetCount(m_Ptr));
		if (!m_Ptr)
			return TRes{ 0 };
		return Traits::GetCount(m_Ptr);
	}

} // namespace brk
