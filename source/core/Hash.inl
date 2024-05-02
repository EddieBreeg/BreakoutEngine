
template <class Int>
constexpr auto brk::Hash<Int, std::enable_if_t<std::is_integral_v<Int>>>::operator()(
	const Int x) const noexcept -> std::make_unsigned_t<Int>
{
	return static_cast<std::make_unsigned_t<Int>>(x);
}

template <class T>
constexpr uint64 brk::CombineHash(uint64 h, T&& val) noexcept(
	IsNoThrowHashable<std::decay_t<T>>)
{
	return h ^= Hash<std::decay_t<T>>{}(val) + 0x9e3779b9 + (h << 6) + (h >> 2);
}