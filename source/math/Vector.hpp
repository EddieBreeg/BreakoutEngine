#pragma once

#include <cmath>
#include <cstdint>

namespace math {
	template <class T>
	struct Vector2;

	template <class T>
	struct Vector3;

	template <class T>
	struct Vector4
	{
		T x = 0, y = 0, z = 0, w = 0;

		constexpr Vector4& operator+=(const Vector4& other) noexcept;

		constexpr Vector4& operator-=(const Vector4& other) noexcept;

		constexpr Vector4& operator*=(const Vector4& other) noexcept;

		constexpr Vector4& operator/=(const Vector4& other);

		constexpr Vector4& operator*=(T s) noexcept;
		constexpr Vector4& operator/=(T s);

		constexpr Vector4 operator-() const noexcept;

		constexpr T Dot(const Vector4& other) const noexcept;
		constexpr T SqrLen() const noexcept;
		constexpr T Len() const noexcept;

		constexpr T SqrDist(const Vector4& other) const noexcept;
		constexpr T Dist(const Vector4& other) const noexcept;

		constexpr Vector4& Normalize();
		constexpr Vector4 Normalized() const;

		constexpr operator Vector2<T>() const noexcept;
		constexpr operator Vector3<T>() const noexcept;
	};

	template <class T>
	struct Vector3
	{
		T x = 0, y = 0, z = 0;

		constexpr Vector3& operator+=(const Vector3& other) noexcept;

		constexpr Vector3& operator-=(const Vector3& other) noexcept;

		constexpr Vector3& operator*=(const Vector3& other) noexcept;

		constexpr Vector3& operator/=(const Vector3& other);

		constexpr Vector3& operator*=(T s) noexcept;
		constexpr Vector3& operator/=(T s);

		constexpr Vector3 operator-() const noexcept;

		constexpr T Dot(const Vector3& other) const noexcept;
		constexpr T SqrLen() const noexcept;
		constexpr T Len() const noexcept;

		constexpr T SqrDist(const Vector3& other) const noexcept;
		constexpr T Dist(const Vector3& other) const noexcept;

		constexpr Vector3 Cross(const Vector3& other) const noexcept;

		constexpr Vector3& Normalize();
		constexpr Vector3 Normalized() const;

		constexpr operator Vector4<T>() const noexcept;
		constexpr operator Vector2<T>() const noexcept;
	};

	template <class T>
	struct Vector2
	{
		T x = 0, y = 0;

		constexpr Vector2& operator+=(const Vector2& other) noexcept;

		constexpr Vector2& operator-=(const Vector2& other) noexcept;

		constexpr Vector2& operator*=(const Vector2& other) noexcept;

		constexpr Vector2& operator/=(const Vector2& other);

		constexpr Vector2& operator*=(T s) noexcept;
		constexpr Vector2& operator/=(T s);

		constexpr Vector2 operator-() const noexcept;

		constexpr T Dot(const Vector2& other) const noexcept;
		constexpr T SqrLen() const noexcept;
		constexpr T Len() const noexcept;

		constexpr T SqrDist(const Vector2& other) const noexcept;
		constexpr T Dist(const Vector2& other) const noexcept;

		constexpr Vector2& Normalize();
		constexpr Vector2 Normalized() const;

		constexpr operator Vector4<T>() const noexcept;
		constexpr operator Vector3<T>() const noexcept;
	};

	namespace _internal {
		template <template <class> class V>
		struct EnableVector;
		template <>
		struct EnableVector<Vector2>
		{
			using Type = int;
		};
		template <>
		struct EnableVector<Vector3>
		{
			using Type = int;
		};
		template <>
		struct EnableVector<Vector4>
		{
			using Type = int;
		};

		template <template <class> class V>
		using EnableIfIsVector = typename EnableVector<V>::Type;
	} // namespace _internal

	template <template <class> class Vec, class T, _internal::EnableIfIsVector<Vec> = 0>
	[[nodiscard]] constexpr Vec<T> operator+(Vec<T> left, const Vec<T>& right) noexcept;

	template <template <class> class Vec, class T, _internal::EnableIfIsVector<Vec> = 0>
	[[nodiscard]] constexpr Vec<T> operator-(Vec<T> left, const Vec<T>& right) noexcept;

	template <template <class> class Vec, class T, _internal::EnableIfIsVector<Vec> = 0>
	[[nodiscard]] constexpr Vec<T> operator*(Vec<T> left, const Vec<T>& right) noexcept;

	template <template <class> class Vec, class T, _internal::EnableIfIsVector<Vec> = 0>
	[[nodiscard]] constexpr Vec<T> operator/(Vec<T> left, const Vec<T>& right);

	template <template <class> class Vec, class T, _internal::EnableIfIsVector<Vec> = 0>
	[[nodiscard]] constexpr Vec<T> operator*(Vec<T> left, T right) noexcept;

	template <template <class> class Vec, class T, _internal::EnableIfIsVector<Vec> = 0>
	[[nodiscard]] constexpr Vec<T> operator*(T left, Vec<T> right) noexcept;

	template <template <class> class Vec, class T, _internal::EnableIfIsVector<Vec> = 0>
	[[nodiscard]] constexpr Vec<T> operator/(Vec<T> left, T right);

	template <class T, class U>
	[[nodiscard]] constexpr bool operator==(const Vector2<T>& a,
											const Vector2<U>& b) noexcept;

	template <class T, class U>
	[[nodiscard]] constexpr bool operator==(const Vector3<T>& a,
											const Vector3<U>& b) noexcept;

	template <class T, class U>
	[[nodiscard]] constexpr bool operator==(const Vector4<T>& a,
											const Vector4<U>& b) noexcept;

	template <template <class> class Vec, class T, class U, _internal::EnableIfIsVector<Vec> = 0>
	[[nodiscard]] constexpr bool operator!=(const Vec<T>& a, const Vec<U>& b) noexcept;

} // namespace math

using float2 = math::Vector2<float>;
using float3 = math::Vector3<float>;
using float4 = math::Vector4<float>;

using int2 = math::Vector2<int32_t>;
using int3 = math::Vector3<int32_t>;
using int4 = math::Vector4<int32_t>;

using uint2 = math::Vector2<uint32_t>;
using uint3 = math::Vector3<uint32_t>;
using uint4 = math::Vector4<uint32_t>;

#include "Vector3.inl"
#include "Vector2.inl"
#include "VectorCommon.inl"