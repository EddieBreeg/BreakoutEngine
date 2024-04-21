#include "Storage.hpp"

namespace bre {
	template <size_t Size, size_t Alignment>
	template <class T, class... Args>
	inline Storage<Size, Alignment>& Storage<Size, Alignment>::Construct(
		Args&&... args)
	{
		static_assert(sizeof(T) <= Size && alignof(T) <= Alignment,
					  "Storage doesn't meet the size/alignment requirements for T");
		new (m_Buf) T{ std::forward<Args>(args)... };
		return *this;
	}

	template <size_t Size, size_t Alignment>
	template <class T>
	inline void Storage<Size, Alignment>::DestroyAs() const
	{
		static_assert(sizeof(T) <= Size && alignof(T) <= Alignment,
					  "Storage doesn't meet the size/alignment requirements for T");
		if constexpr (std::is_trivially_destructible_v<T>)
			return;

		reinterpret_cast<const T*>(m_Buf)->~T();
	}

	template <size_t Size, size_t Alignment>
	template <class T>
	inline T& bre::Storage<Size, Alignment>::GetAs()
	{
		static_assert(sizeof(T) <= Size && alignof(T) <= Alignment,
					  "Storage doesn't meet the size/alignment requirements for T");
		return *reinterpret_cast<T*>(m_Buf);
	}

	template <size_t Size, size_t Alignment>
	template <class T>
	inline const T& bre::Storage<Size, Alignment>::GetAs() const
	{
		static_assert(sizeof(T) <= Size && alignof(T) <= Alignment,
					  "Storage doesn't meet the size/alignment requirements for T");
		return *reinterpret_cast<const T*>(m_Buf);
	}

} // namespace bre
