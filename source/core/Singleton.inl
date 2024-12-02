#include "Assert.hpp"

namespace brk {
	template <class T>
	template <class... Args>
	inline T& Singleton<T>::Init(Args&&... args)
	{
		if (T::s_Instance)
			return *T::s_Instance;
		T::s_Instance.reset(new T{ std::forward<Args>(args)... });
		return *T::s_Instance;
	}

	template <class T>
	inline T& brk::Singleton<T>::GetInstance()
	{
		BRK_ASSERT(T::s_Instance,
						"Called GetInstance on a uninitialized singleton");
		return *T::s_Instance;
	}

	template <class T>
	inline void Singleton<T>::Reset()
	{
		if (!T::s_Instance)
			return;
		T::s_Instance.reset();
	}
} // namespace brk
