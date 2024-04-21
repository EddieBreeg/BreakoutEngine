#include "Assert.hpp"

namespace bre {
	template <class T>
	std::unique_ptr<T> Singleton<T>::s_Instance = nullptr;

	template <class T>
	template <class... Args>
	inline T& Singleton<T>::Init(Args&&... args)
	{
		if (s_Instance)
			return *s_Instance;
		s_Instance.reset(new T{ std::forward<Args>(args)... });
		return *s_Instance;
	}

	template <class T>
	inline T& bre::Singleton<T>::GetInstance()
	{
		BREAKOUT_ASSERT(s_Instance,
						"Called GetInstance on a uninitialized singleton");
		return *s_Instance;
	}

	template <class T>
	inline void Singleton<T>::Reset()
	{
		if (!s_Instance)
			return;
		s_Instance.reset();
	}
} // namespace bre
