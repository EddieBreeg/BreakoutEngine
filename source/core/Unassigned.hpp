#pragma once

namespace brk
{
	template<class T>
	struct Unassigned_t;

	template<class T>
	static constexpr T Unassigned = Unassigned_t<T>::Value;

	template<class T>
	struct Unassigned_t<T*>
	{
		static constexpr T* Value = nullptr;
	};
} // namespace brk
