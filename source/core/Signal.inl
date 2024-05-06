#include "Signal.hpp"
template <class R, class... Args>
template <class F>
void brk::Signal<R(Args...)>::operator+=(F&& callback)
{
	m_Callbacks.emplace_back(std::forward<F>(callback));
}

template <class R, class... Args>
void brk::Signal<R(Args...)>::Emit(Args... args)
{
	for (auto& cbk : m_Callbacks)
		cbk(args...);
}

template <class R, class... Args>
template <class F, bool _B, std::enable_if_t<_B, int>>
void brk::Signal<R(Args...)>::Emit(F&& forEach, Args... args)
{
	static_assert(std::is_invocable_v<F, R>, "Invalid callback type");
	for (auto& cbk : m_Callbacks)
		forEach(cbk(args...));
}
