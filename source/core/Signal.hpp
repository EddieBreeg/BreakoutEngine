#pragma once

#include <PCH.hpp>
#include "Function.hpp"
#include <vector>

namespace brk {
	/**
	 * Represents a connection between an event and a collection of callbacks
	 * \tparam F The function signature for the callbacks
	 */
	template <class F>
	class Signal;

	template <class R, class... Args>
	class Signal<R(Args...)>
	{
	public:
		Signal() noexcept = default;

		/**
		 * Registers a new callback, which will be called whenever the signal is emitted
		 */
		template <class F>
		void Connect(F&& callback);

		/**
		 * Invokes all connected callbacks using the provied arguments.
		 * Any return values are ignored
		 */
		void Emit(Args... args);

		/**
		 * Invokes all connected callbacks with the provided arguments. This function
		 * only participates to overload resolution if R is void and forEach is invocable
		 * with an object of type R
		 * \param forEach A function-like object which will be called with the value
		 * returned from each callback
		 * \param args The arguments to pass into each callback
		 */
		template <
			class F,
			bool _B = !std::is_void_v<R> && std::is_invocable_v<F, R>,
			std::enable_if_t<_B, int> = 0>
		void Emit(F&& forEach, Args... args);

	private:
		std::vector<UniqueFunction<R(Args...)>> m_Callbacks;
	};
} // namespace brk

#include "Signal.inl"