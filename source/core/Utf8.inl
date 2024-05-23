template <class Int, uint8 Base, std::enable_if_t<std::is_integral_v<Int>, int>>
constexpr std::optional<Int> brk::utf8::FromString(StringView str) noexcept
{
	if (str.IsEmpty())
		return {};

	Int result = 0;

	int8 sign = 1;
	if (*(str.begin()) == '-')
	{
		sign = -1;
		str.TrimLeft(1);
	}

	for (const char c : str)
	{
		const Int m = FromChar<Int>(c);
		if (m < 0 || m >= Base)
			return {};
		result = (result * Base) + m;
	}
	return result * sign;
}

/**
 * Converts a single character into a integral value, if said character can be interpreted
 * as a digit.
 * \return If c is between '0' and '9', the digit represented by c is returned.
 * If c is a letter (a-z or A-Z), the index of c in the alphabet is returned.
 * Otherwise, static_cast<Int>(-1) is returned.
 */
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
