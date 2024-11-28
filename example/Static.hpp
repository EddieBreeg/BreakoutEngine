#pragma once

#include <core/StringView.hpp>
#include <core/ULID.hpp>

namespace brk::sandbox
{
	constexpr StringView s_FragShaderSource = R"hlsl(
cbuffer Params: register(b0)
{
	float4 DiffuseColor;
}

float4 fs_main(float4 fragPos: SV_POSITION): SV_TARGET
{
	return DiffuseColor;
}
)hlsl";

	constexpr ULID s_MaterialId = ULID::FromString("01JDS6Q072ERV1WXSSCFKRS8CY");
} // namespace brk::sandbox
