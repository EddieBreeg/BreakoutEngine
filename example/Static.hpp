#pragma once

#include <core/StringView.hpp>
#include <core/ULID.hpp>

namespace brk::sandbox {
	constexpr StringView s_FragShaderSource = R"hlsl(
cbuffer Params: register(b0)
{
	float4 DiffuseColor;
}

Texture2D g_DiffuseMap: register(t0);
SamplerState g_Sampler: register(s0);

struct Fragment
{
	float4 Position: SV_POSITION;
	float3 Normal: NORMAL;
	float2 UV: TEXCOORD;
};

float4 fs_main(Fragment frag): SV_TARGET
{
	const float2 uv = float2(frag.UV.x, 1 - frag.UV.y);
	return DiffuseColor * g_DiffuseMap.Sample(g_Sampler, uv);
}
)hlsl";

	constexpr ULID s_MaterialId = ULID::FromString("01JDS6Q072ERV1WXSSCFKRS8CY");
	constexpr ULID s_TextureId = ULID::FromString("01JDW4CZM6YB7J6ZJNPMC60THK");
} // namespace brk::sandbox
