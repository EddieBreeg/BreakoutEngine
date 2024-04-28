#include "Vector.hpp"

namespace math {
	template <class T>
	constexpr Vector4<T>& Vector4<T>::operator+=(const Vector4& other) noexcept
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	template <class T>
	constexpr Vector4<T>& Vector4<T>::operator-=(const Vector4& other) noexcept
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	template <class T>
	constexpr Vector4<T>& Vector4<T>::operator*=(const Vector4& other) noexcept
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	template <class T>
	constexpr Vector4<T>& Vector4<T>::operator/=(const Vector4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	template <class T>
	inline constexpr Vector4<T>& Vector4<T>::operator*=(T s) noexcept
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	template <class T>
	inline constexpr Vector4<T>& Vector4<T>::operator/=(T s)
	{
		x /= s;
		y /= s;
		z /= s;
		w /= s;
		return *this;
	}

	template <class T>
	inline constexpr Vector4<T> Vector4<T>::operator-() const noexcept
	{
		return { -x, -y, -z, -w };
	}

	template <class T>
	inline constexpr T math::Vector4<T>::Dot(const Vector4& other) const noexcept
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	template <class T>
	inline constexpr T Vector4<T>::SqrLen() const noexcept
	{
		return Dot(*this);
	}

	template <class T>
	inline constexpr T Vector4<T>::Len() const noexcept
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
	inline constexpr T Vector4<T>::SqrDist(const Vector4& other) const noexcept
	{
		return (*this - other).SqrLen();
	}

	template <class T>
	inline constexpr T Vector4<T>::Dist(const Vector4& other) const noexcept
	{
		return (*this - other).Len();
	}

	template <class T>
	inline constexpr Vector4<T>& Vector4<T>::Normalize()
	{
		return (*this /= Len());
	}

	template <class T>
	inline constexpr Vector4<T> Vector4<T>::Normalized() const
	{
		return Vector4{ x, y, z, w } / Len();
	}

	template <class T>
	inline constexpr Vector4<T>::operator Vector2<T>() const noexcept
	{
		return { x, y };
	}

	template <class T>
	inline constexpr Vector4<T>::operator Vector3<T>() const noexcept
	{
		return { x, y, z };
	}

	template <class T, class U>
	constexpr bool operator==(const Vector4<T>& a, const Vector4<U>& b) noexcept
	{
		return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}
} // namespace math
