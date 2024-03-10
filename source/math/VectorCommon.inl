namespace math {
	template <template <class> class Vec, class T>
	constexpr Vec<T> operator+(Vec<T> left, const Vec<T>& right) noexcept
	{
		left += right;
		return left;
	}

	template <template <class> class Vec, class T>
	constexpr Vec<T> operator-(Vec<T> left, const Vec<T>& right) noexcept
	{
		left -= right;
		return left;
	}

	template <template <class> class Vec, class T>
	constexpr Vec<T> operator*(Vec<T> left, const Vec<T>& right) noexcept
	{
		left *= right;
		return left;
	}

	template <template <class> class Vec, class T>
	constexpr Vec<T> operator/(Vec<T> left, const Vec<T>& right)
	{
		left /= right;
		return left;
	}

	template <template <class> class Vec, class T>
	constexpr Vec<T> operator*(Vec<T> left, T right) noexcept
	{
		left *= right;
		return left;
	}

	template <template <class> class Vec, class T>
	constexpr Vec<T> operator*(T left, Vec<T> right) noexcept
	{
		right *= left;
		return right;
	}

	template <template <class> class Vec, class T>
	constexpr Vec<T> operator/(Vec<T> left, T right)
	{
		left /= right;
		return left;
	}

	template <template <class> class Vec, class T>
	constexpr Vec<T> operator/(T left, Vec<T> right)
	{
		right /= left;
		return right;
	}

	template <template <class> class Vec, class T, class U>
	constexpr bool operator!=(const Vec<T>& a, const Vec<U>& b) noexcept
	{
		return !(a == b);
	}
} // namespace math
