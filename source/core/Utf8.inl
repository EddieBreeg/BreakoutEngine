template <class Int, uint8 Base, std::enable_if_t<std::is_integral_v<Int>, int>>
constexpr std::optional<Int> brk::utf8::FromString(const StringView str) noexcept
{
	Int result = 0;
	for (const char c : str)
	{
		const Int m = FromChar<Int>(c);
		if (m == -1)
			return {};
		result = (result * Base) + m;
	}
	return result;
}

template <class Int, std::enable_if_t<std::is_integral_v<Int>, int>>
constexpr Int brk::utf8::FromChar(const char c) noexcept
{
	if ('0' <= c && c <= '9')
		return c - '0';
	else if ('a' <= c && c <= 'z')
		return c - 87;
	else if ('A' <= c && c <= 'Z')
		return c - 55;

	return static_cast<Int>(-1);
}
