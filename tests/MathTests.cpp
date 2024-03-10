#include <math/Vector.hpp>
#include <cassert>

int main(int argc, char const* argv[])
{
	static_assert((float3{ 0, 1 } + float3{ 1 }) == float3{ 1, 1, 0 });
	static_assert((float3{ 0, 1 } - float3{ 1 }) == float3{ -1, 1, 0 });
	static_assert((float3{ 0, 1 } * float3{ 1 }) == float3{});
	static_assert((float3{ 4, 4, 4 } / float3{ 2, 2, 2 }) == float3{ 2, 2, 2 });
	static_assert((2.0f * float3{ 1, 0, 1 }) == float3{ 2, 0, 2 });
	static_assert((float3{ 1, 0, 1 } / 2.0f) == float3{ .5f, 0, .5f });

	static_assert(float3{ 1, 1 }.Dot(float3{ 1, 1, 1 }) == 2);
	static_assert(float3{ 1, 1 }.SqrLen() == 2);

	static_assert(float3{}.SqrDist({ 1 }) == 1.0f);

	static_assert(float3{ 1 }.Cross({ 0, 1 }) == float3{ 0, 0, 1 });
	static_assert(float3{ 0, 1 }.Cross({ 0, 0, 1 }) == float3{ 1, 0, 0 });
	static_assert(float3{ 0, 0, 1 }.Cross({ 1 }) == float3{ 0, 1 });
	static_assert(float3{ 0, 0, 1 }.Cross({ 0, 1 }) == -float3{ 1 });
	static_assert(float3{ 0, 1 }.Cross({ 1 }) == -float3{ 0, 0, 1 });
	static_assert(float3{ 1 }.Cross({ 0, 0, 1 }) == -float3{ 0, 1 });
}
