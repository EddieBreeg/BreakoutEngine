#pragma once

#include "Vector.hpp"
#include <cmath>

namespace math {
	class Quaternion
	{
	public:
		constexpr Quaternion() = default;
		constexpr Quaternion(float x, float y, float z, float w);
		constexpr Quaternion(const float4 vec);
		constexpr Quaternion(const float3 vec);
		constexpr Quaternion(const Quaternion&) = default;
		~Quaternion() = default;
		constexpr Quaternion& operator=(const Quaternion&) = default;

		static Quaternion AxisAngle(float3 axis, float angle);

		[[nodiscard]] static constexpr Quaternion Identity() noexcept
		{
			return Quaternion{};
		};

		[[nodiscard]] static float3 Rotate(Quaternion q, float3 v, bool normalize = false);

		[[nodiscard]] constexpr operator float3() const noexcept;

		constexpr Quaternion Conjugate() const noexcept;

		[[nodiscard]] constexpr float SquaredNorm() const noexcept;
		[[nodiscard]] float Norm() const noexcept;

		[[nodiscard]] Quaternion& Normalize();
		[[nodiscard]] Quaternion Normalized();

		[[nodiscard]] constexpr Quaternion Inverse() const;

		constexpr Quaternion operator*(const Quaternion other) const noexcept;
		constexpr Quaternion operator*(float x) const noexcept;
		constexpr Quaternion operator/(float scale) const;

		[[nodiscard]] constexpr float GetX() const noexcept { return x; }
		[[nodiscard]] constexpr float GetY() const noexcept { return y; }
		[[nodiscard]] constexpr float GetZ() const noexcept { return z; }
		[[nodiscard]] constexpr float GetW() const noexcept { return w; }

		[[nodiscard]] constexpr bool operator==(const Quaternion other) const noexcept;
		[[nodiscard]] constexpr bool operator!=(const Quaternion other) const noexcept
		{
			return !(*this == other);
		}

	private:
		float x = 0, y = 0, z = 0, w = 1;
	};
} // namespace math

#include "Quaternion.inl"