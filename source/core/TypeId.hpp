#pragma once

#include <PCH.hpp>

namespace brk {
	/**
	 * Uniquely identifies a type.
	 */
	struct TypeId
	{
		uint32 m_Index;
		uint32 m_Size = 0;
		uint32 m_Alignment = 0;

		template <class T>
		static const TypeId& Get() noexcept
		{
			return s_Id<T>;
		}

		[[nodiscard]] bool operator==(const TypeId& other) const noexcept
		{
			return this == &other;
		}
		[[nodiscard]] bool operator!=(const TypeId& other) const noexcept
		{
			return this != &other;
		}

	private:
		template <class T>
		TypeId(std::in_place_type_t<T>)
			: m_Index{ s_Count++ }
		{
			if constexpr (!std::is_void_v<T>)
			{
				m_Size = sizeof(T);
				m_Alignment = alignof(T);
			}
		}

		static inline uint32 s_Count = 0;

		template <class T>
		static const TypeId s_Id;
	};

	template <class T>
	const TypeId TypeId::s_Id{ std::in_place_type<T> };
} // namespace brk
