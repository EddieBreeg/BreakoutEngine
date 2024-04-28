#include "Vector.hpp"

namespace math {
	template <class T>
	constexpr Vector2<T>& Vector2<T>::operator+=(const Vector2& other) noexcept
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	template <class T>
	constexpr Vector2<T>& Vector2<T>::operator-=(const Vector2& other) noexcept
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template <class T>
	constexpr Vector2<T>& Vector2<T>::operator*=(const Vector2& other) noexcept
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	template <class T>
	constexpr Vector2<T>& Vector2<T>::operator/=(const Vector2& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	template <class T>
	inline constexpr Vector2<T>& Vector2<T>::operator*=(T s) noexcept
	{
		x *= s;
		y *= s;
		return *this;
	}

	template <class T>
	inline constexpr Vector2<T>& Vector2<T>::operator/=(T s)
	{
		x /= s;
		y /= s;
		return *this;
	}

	template <class T>
	inline constexpr Vector2<T> Vector2<T>::operator-() const noexcept
	{
		return { -x, -y };
	}

	template <class T>
	inline constexpr T math::Vector2<T>::Dot(const Vector2& other) const noexcept
	{
		return x * other.x + y * other.y;
	}

	template <class T>
	inline constexpr T Vector2<T>::SqrLen() const noexcept
	{
		return Dot(*this);
	}

	template <class T>
	inline constexpr T Vector2<T>::Len() const noexcept
	{
		if constexpr (std::is_same_v<T, double>)
			return sqrt(SqrLen());
		else if constexpr (std::is_same_v<T, long double>)
			return sqrtl(SqrLen());
		else if constexpr (std::is_same_v<T, float>)
			return sqrtf(SqrLen());
		else
			return std::sqrt<T>(SqrLen());
	}

	template <class T>
	inline constexpr T Vector2<T>::SqrDist(const Vector2& other) const noexcept
	{
		return (*this - other).SqrLen();
	}

	template <class T>
	inline constexpr T Vector2<T>::Dist(const Vector2& other) const noexcept
	{
		return (*this - other).Len();
	}

	template <class T>
	inline constexpr Vector2<T>& Vector2<T>::Normalize()
	{
		return (*this /= Len());
	}

	template <class T>
	inline constexpr Vector2<T> Vector2<T>::Normalized() const
	{
		return Vector2{ x, y } / Len();
	}

	template <class T>
	inline constexpr Vector2<T>::operator Vector4<T>() const noexcept
	{
		return { x, y };
	}

	template <class T>
	inline constexpr Vector2<T>::operator Vector3<T>() const noexcept
	{
		return { x, y };
	}

	template <class T, class U>
	constexpr bool operator==(const Vector2<T>& a, const Vector2<U>& b) noexcept
	{
		return a.x == b.x && a.y == b.y;
	}
} // namespace math
