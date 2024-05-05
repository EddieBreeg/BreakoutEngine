template <class Cbk, class... Args>
void brk::ThreadPool::EnqueueTask(Cbk&& callback, Args&&... args)
{
	static_assert(
		std::is_void_v<std::invoke_result_t<Cbk, Args...>>,
		"Wrong callback signature");
	if constexpr (!sizeof...(Args))
	{
		std::unique_lock lock{ m_Mutex };
		m_Tasks.emplace(std::forward<Cbk>(callback));
	}
	else
	{
		static_assert(
			(!std::is_lvalue_reference_v<Args> && ...),
			"lvalue references not allowed, use a reference wrapper if you really need "
			"to");
		using TArgs = std::tuple<std::decay_t<Args...>>;

		std::unique_lock lock{ m_Mutex };
		m_Tasks.emplace(
			[f = std::move(callback), tuple = TArgs{ std::move(args)... }]() mutable
			{
				ThreadPool::Apply(
					std::move(f),
					std::move(tuple),
					std::make_index_sequence<sizeof...(Args)>{});
			});
	}
	m_Cv.notify_one();
}

template <class F, class... Args, size_t... I>
void brk::ThreadPool::Apply(
	F&& func,
	std::tuple<Args...>&& args,
	std::index_sequence<I...>)
{
	func(std::move(std::get<I>(args))...);
}