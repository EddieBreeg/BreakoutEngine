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
struct Fragment
{
	float4 Position: SV_POSITION;
	float3 Normal: NORMAL;
	float2 Uv: TEXCOORD;
};

cbuffer GlobalFrameData: register(b1)
{
	float4x4 ViewProjection;
}

cbuffer ObjectTransform: register(b2)
{
	float4x4 Model;
}

Fragment vs_main(Vertex v)
{
	Fragment result;
	result.Position = mul(mul(float4(v.Position, 1), Model), ViewProjection);
	result.Normal = v.Normal;
	result.Uv = v.Uv;
	return result;
}

float4 fs_main(Fragment frag): SV_TARGET
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
		VertexShader(VertexShader&& other)
			: BasicResource(std::move(other))
			, m_ByteCode{ other.m_ByteCode }
		{
			other.m_ByteCode = nullptr;
		}

		VertexShader& operator=(VertexShader&& other)
		{
			std::swap(m_Handle, other.m_Handle);
			std::swap(m_ByteCode, other.m_ByteCode);
			return *this;
		}

		void* GetByteCode() noexcept { return m_ByteCode; }

	private:
		void* m_ByteCode = nullptr;
	};

	class FragmentShader : public BasicResource<FragmentShader>
	{
	public:
		/* Constructs the default fragment shader (compile from s_DefaultShaderCode)*/
		FragmentShader();
		FragmentShader(StringView source, const char* entryPoint = "fs_main");
		~FragmentShader();

		FragmentShader(FragmentShader&& other)
			: BasicResource(std::move(other))
			, m_ByteCode{ other.m_ByteCode }
		{
			other.m_ByteCode = nullptr;
		}

		FragmentShader& operator=(FragmentShader&& other)
		{
			std::swap(m_Handle, other.m_Handle);
			std::swap(m_ByteCode, other.m_ByteCode);
			return *this;
		}

		void* GetByteCode() noexcept { return m_ByteCode; }

	private:
		void* m_ByteCode = nullptr;
	};
} // namespace brk::rdr
