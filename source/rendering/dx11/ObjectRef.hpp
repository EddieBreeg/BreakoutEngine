#pragma once

#include <PCH.hpp>

namespace brk::rdr::d3d {
	template <class T>
	struct ObjectDeleter
	{
		void operator()(T* obj) const { obj->Release(); }
	};

	template <class T, class D = ObjectDeleter<T>>
	struct Ref
	{
		T* m_Handle = nullptr;
		constexpr Ref() = default;
		Ref(T* ptr)
			: m_Handle{ ptr }
		{}
		Ref(const Ref&) = delete;
		Ref(Ref&& other) noexcept
			: m_Handle{ other.m_Handle }
		{
			other.m_Handle = nullptr;
		}
		Ref& operator=(const Ref&) = delete;
		Ref& operator=(Ref&& other) noexcept
		{
			std::swap(m_Handle, other.m_Handle);
			return *this;
		}
		T* operator->() noexcept { return m_Handle; }
		const T* operator->() const noexcept { return m_Handle; }
		[[nodiscard]] operator T*() noexcept { return m_Handle; }
		[[nodiscard]] operator const T*() const noexcept { return m_Handle; }

		T* Release() noexcept { return std::exchange(m_Handle, nullptr); }
		void Reset(T* newHandle = nullptr) noexcept
		{
			if (m_Handle)
				D{}(m_Handle);
			m_Handle = newHandle;
		}
		void Swap(Ref& other) { std::swap(other.m_Handle, m_Handle); }

		~Ref()
		{
			if (m_Handle)
				D{}(m_Handle);
		}
	};

	template <class T>
	[[nodiscard]] bool operator==(const Ref<T>& a, const Ref<T>& b)
	{
		return a.m_Handle == b.m_Handle;
	}
	template <class T>
	[[nodiscard]] bool operator!=(const Ref<T>& a, const Ref<T>& b)
	{
		return a.m_Handle != b.m_Handle;
	}
} // namespace brk::rdr::d3d
