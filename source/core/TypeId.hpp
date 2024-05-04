#pragma once

#include <PCH.hpp>

namespace brk {
	template <class Family>
	struct BasicTypeId
	{
		const uint32 m_Index;
		const uint32 m_Size;
		const uint32 m_Alignment;

		template <class T>
		static const BasicTypeId& Get() noexcept
		{
			if constexpr (std::is_void_v<T>)
				return VoidWrapper<void>::s_Id;
			else
				return Wrapper<T>::s_Id;
		}

	private:
		template <class T>
		struct Wrapper
		{
			static const BasicTypeId s_Id;
		};

		template <class = void>
		struct VoidWrapper
		{
			static const BasicTypeId s_Id;
		};

		BasicTypeId(const uint32 index);
		BasicTypeId(
			const uint32 index,
			const uint32 size,
			const uint32 alignment) noexcept;
		BasicTypeId(const BasicTypeId&) = default;

		static uint32 s_Count;
	};

	template <class F>
	uint32 BasicTypeId<F>::s_Count = 0;

	template <class F>
	template <class T>
	const BasicTypeId<F> BasicTypeId<F>::Wrapper<T>::s_Id{
		BasicTypeId<F>::s_Count++,
		sizeof(T),
		alignof(T),
	};

	template <class F>
	template <class T>
	const BasicTypeId<F> BasicTypeId<F>::VoidWrapper<T>::s_Id{
		BasicTypeId<F>::s_Count++
	};

	template <class Family>
	BasicTypeId<Family>::BasicTypeId(
		const uint32 index,
		const uint32 size,
		const uint32 alignment) noexcept
		: m_Index{ index }
		, m_Size{ size }
		, m_Alignment{ alignment }
	{}

	template <class Family>
	BasicTypeId<Family>::BasicTypeId(const uint32 index)
		: m_Index{ index }
		, m_Size{ 0 }
		, m_Alignment{ 0 }
	{}

	using TypeId = BasicTypeId<void>;
} // namespace brk
