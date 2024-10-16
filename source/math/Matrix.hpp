#pragma once

#include "Vector.hpp"
#include <glm/mat4x4.hpp>

namespace math {
	using Matrix44 = glm::mat4x4;
} // namespace math

using float4x4 = math::Matrix44;