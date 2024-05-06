#pragma once

#include <PCH.hpp>
#include "Function.hpp"
#include <vector>

namespace brk {
	template <class F>
	class Signal;

	template <class R, class... Args>
	class Signal<R(Args...)>
	{
	public:
		Signal() noexcept = default;

		template<class F>
		void operator+=(F&& callback);

		void Emit(Args... args);

		template<class F, bool _B = !std::is_void_v<R>, std::enable_if_t<_B, int> = 0>
		void Emit(F&& forEach, Args... args);

	private:
		std::vector<UniqueFunction<R(Args...)>> m_Callbacks;
	};
} // namespace brk

#include "Signal.inl"