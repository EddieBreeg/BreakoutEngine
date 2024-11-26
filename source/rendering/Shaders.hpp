#pragma once

#include <core/StringView.hpp>
#include "Resource.hpp"

namespace brk::rdr {
	/**
	 * Default shader implementation. This automatically gets compile to byte code
	 * by the renderer upon initialization.
	 */
	constexpr const char* s_DefaultShaderCode = R"hlsl(
struct Vertex
{
	float3 Position: POSITION;
	float3 Normal: NORMAL;
	float2 Uv: TEXCOORD;
};

float4 vs_main(Vertex v): SV_POSITION
{
	return float4(v.Position, 1);
}

float4 fs_main(float4 fragPos: SV_POSITION): SV_TARGET
{
	return float4(1, 1, 1, 1);
}
)hlsl";

	class VertexShader : public BasicResource<VertexShader>
	{
	public:
		/* Constructs the default vertex shader (compile from s_DefaultShaderCode)*/
		VertexShader();
		VertexShader(StringView source, const char* entryPoint = "vs_main");
		~VertexShader();

		void* GetByteCode() noexcept { return m_ByteCode; }

	private:
		void* m_ByteCode = nullptr;
	};

	class FragmentShader : public BasicResource<FragmentShader>
	{
	public:
		/* Constructs the default fragment shader (compile from s_DefaultShaderCode)*/
		FragmentShader();
		FragmentShader(StringView source, const char* entryPoint = "ps_main");
		~FragmentShader();

		void* GetByteCode() noexcept { return m_ByteCode; }

	private:
		void* m_ByteCode = nullptr;
	};
} // namespace brk::rdr
