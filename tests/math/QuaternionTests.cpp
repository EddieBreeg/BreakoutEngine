#pragma once

#include <math/Quaternion.hpp>
#include <math/Constants.hpp>
#include <cassert>
#include <iostream>

namespace {
	bool ApproxEqual(const float x, const float y, const float margin = .01f)
	{
		const float d = y - x;
		return (-margin <= d) && d <= margin;
	}

	bool ApproxEqual(const float3 a, const float3 b)
	{
		return ApproxEqual(a.x, b.x) && ApproxEqual(a.y, b.y) && ApproxEqual(a.z, b.z);
	}
} // namespace

namespace math::ut {
	void CompileTimeTests()
	{
		constexpr Quaternion q = Quaternion::Identity();
		static_assert(q.GetX() == 0);
		static_assert(q.GetY() == 0);
		static_assert(q.GetZ() == 0);
		static_assert(q.GetW() == 1);
		static_assert(q == Quaternion{});

		static_assert(q.SquaredNorm() == 1);

		constexpr Quaternion q2{ 1, 1, 1, 0 };
		static_assert(q * q2 == q2);
		static_assert(q2 * q == q2);
		static_assert(q2 * 2 == Quaternion{ 2, 2, 2, 0 });

		static_assert(q2 * q2.Inverse() == Quaternion::Identity());

		{
			constexpr Quaternion a{ 2, 3, 4, 1 }, b{ 6, 7, 8, 5 };
			constexpr Quaternion p1 = a * b;
			constexpr Quaternion p2 = b * a;
			static_assert(p1 == Quaternion{ 12, 30, 24, -60 });
			static_assert(p2 == Quaternion{ 20, 14, 32, -60 });
		}
	}

	void QuaternionRotationTests()
	{
		{
			const Quaternion q = Quaternion::AxisAngle({ 0, 0, 1 }, Pi);
			const float3 r = Quaternion::Rotate(q, { 1, 0, 0 });
			assert(ApproxEqual(r, { -1, 0, 0 }));
		}
		{
			const Quaternion q = Quaternion::AxisAngle({ 0, 0, 1 }, PiOver2);
			const float3 r = Quaternion::Rotate(q, { 1, 0, 0 });
			assert(ApproxEqual(r, { 0, 1, 0 }));
		}
	}
} // namespace math::ut
