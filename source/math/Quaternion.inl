#include "Quaternion.hpp"
namespace math {
	constexpr Quaternion::Quaternion(float x, float y, float z, float w)
		: x{ x }
		, y{ y }
		, z{ z }
		, w{ w }
	{}

	inline constexpr Quaternion::Quaternion(const float4 vec)
		: x{ vec.x }
		, y{ vec.y }
		, z{ vec.z }
		, w{ vec.w }
	{}

	inline constexpr Quaternion::Quaternion(const float3 vec)
		: x{ vec.x }
		, y{ vec.y }
		, z{ vec.z }
		, w{ 0 }
	{}

	inline Quaternion Quaternion::AxisAngle(float3 axis, float angle, bool normalize)
	{
		angle /= 2.0f;
		if (normalize)
			axis.Normalize();
		const float w = std::cosf(angle);
		const float s = std::sinf(angle);
		return { s * axis.x, s * axis.y, s * axis.z, w };
	}

	inline float3 Quaternion::Rotate(Quaternion q, float3 v)
	{
		Quaternion p{ v };
		return q * p * q.Inverse();
	}

	inline constexpr Quaternion::operator float3() const noexcept
	{
		return { x, y, z };
	}

	inline constexpr Quaternion Quaternion::Conjugate() const noexcept
	{
		return Quaternion{ -x, -y, -z, w };
	}

	inline constexpr float Quaternion::SquaredNorm() const noexcept
	{
		return x * x + y * y + z * z + w * w;
	}

	inline float Quaternion::Norm() const noexcept
	{
		return std::sqrtf(SquaredNorm());
	}

	inline Quaternion& Quaternion::Normalize()
	{
		const float n = Norm();
		x /= n;
		y /= n;
		z /= n;
		w /= n;
		return *this;
	}

	inline Quaternion Quaternion::Normalized()
	{
		return *this / Norm();
	}

	inline constexpr Quaternion Quaternion::Inverse() const
	{
		return Conjugate() / SquaredNorm();
	}

	inline constexpr Quaternion math::Quaternion::operator*(
		const Quaternion other) const noexcept
	{
		return { w * other.x + x * other.w + y * other.z - z * other.y,
				 w * other.y + y * other.w + z * other.x - x * other.z,
				 w * other.z + z * other.w + x * other.y - y * other.x,
				 w * other.w - x * other.x - y * other.y - z * other.z };
	}

	inline constexpr Quaternion Quaternion::operator*(float s) const noexcept
	{
		return { x * s, y * s, z * s, w * s };
	}

	inline constexpr Quaternion Quaternion::operator/(float s) const
	{
		return { x / s, y / s, z / s, w / s };
	}

	inline constexpr bool math::Quaternion::operator==(
		const Quaternion other) const noexcept
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

} // namespace math
