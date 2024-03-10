#include "Vector.hpp"

namespace math {
	template <class T>
	constexpr Vector3<T>& Vector3<T>::operator+=(const Vector3& other) noexcept
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	template <class T>
	constexpr Vector3<T>& Vector3<T>::operator-=(const Vector3& other) noexcept
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	template <class T>
	constexpr Vector3<T>& Vector3<T>::operator*=(const Vector3& other) noexcept
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	template <class T>
	constexpr Vector3<T>& Vector3<T>::operator/=(const Vector3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	template <class T>
	inline constexpr Vector3<T>& Vector3<T>::operator*=(T s) noexcept
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	template <class T>
	inline constexpr Vector3<T>& Vector3<T>::operator/=(T s)
	{
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	template <class T>
	inline constexpr Vector3<T> Vector3<T>::operator-() const noexcept
	{
		return { -x, -y, -z };
	}

	template <class T>
	inline constexpr T math::Vector3<T>::Dot(const Vector3& other) const noexcept
	{
		return x * other.x + y * other.y + z * other.z;
	}

	template <class T>
	inline constexpr T Vector3<T>::SqrLen() const noexcept
	{
		return Dot(*this);
	}

	template <class T>
	inline constexpr T Vector3<T>::Len() const noexcept
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
	inline constexpr T Vector3<T>::SqrDist(const Vector3& other) const noexcept
	{
		return (*this - other).SqrLen();
	}

	template <class T>
	inline constexpr T Vector3<T>::Dist(const Vector3& other) const noexcept
	{
		return (*this - other).Len();
	}

	template <class T>
	inline constexpr Vector3<T> Vector3<T>::Cross(
		const Vector3& other) const noexcept
	{
		return {
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x,
		};
	}

	template <class T>
	inline constexpr Vector3<T>& Vector3<T>::Normalize()
	{
		return (*this /= Len());
	}

	template <class T>
	inline constexpr Vector3<T> Vector3<T>::Normalized() const
	{
		return Vector3{ x, y, z } / Len();
	}

	template <class T>
	inline constexpr Vector3<T>::operator Vector2<T>() const noexcept
	{
		return { x, y };
	}

	template <class T, class U>
	constexpr bool operator==(const Vector3<T>& a, const Vector3<U>& b) noexcept
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
} // namespace math
