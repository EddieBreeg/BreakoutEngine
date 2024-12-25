#include <rendering/Shaders.hpp>
#include <rendering/Renderer.hpp>
#include "Renderer.hpp"
#include "Shaders.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <core/LogManager.hpp>
#include <comdef.h>

namespace {
	template <class S>
	struct ShaderTypeInfo;

	template <>
	struct ShaderTypeInfo<ID3D11VertexShader>
	{
		static constexpr auto Constructor = &ID3D11Device::CreateVertexShader;
		static constexpr const char* Name = "vertex shader";
	};

	template <>
	struct ShaderTypeInfo<ID3D11PixelShader>
	{
		static constexpr auto Constructor = &ID3D11Device::CreatePixelShader;
		static constexpr const char* Name = "fragment shader";
	};

	template <class S>
	S* CreateShader(ID3D11Device* device, ID3DBlob* byteCode)
	{
		S* shader = nullptr;
		using TInfo = ShaderTypeInfo<S>;
		constexpr auto ctor = TInfo::Constructor;

		const HRESULT res = (device->*ctor)(
			byteCode->GetBufferPointer(),
			byteCode->GetBufferSize(),
			nullptr,
			&shader);
		DEBUG_CHECK(res == S_OK)
		{
			BRK_LOG_ERROR(
				"Failed to create {}: {}",
				TInfo::Name,
				_com_error(res).ErrorMessage());
		}
		return shader;
	}
} // namespace

ID3DBlob* brk::rdr::d3d::CompileShader(
	StringView source,
	const char* target,
	const char* entryPoint)
{
	ID3DBlob *byteCode = nullptr, *logs = nullptr;
	uint32 flags = 0;
#if BRK_DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif
	D3DCompile(
		source.GetPtr(),
		source.GetLen(),
		nullptr,
		nullptr,
		nullptr,
		entryPoint,
		target,
		flags,
		0,
		&byteCode,
		&logs);

	DEBUG_CHECK(!logs)
	{
		const StringView message{
			static_cast<const char*>(logs->GetBufferPointer()),
			uint32(logs->GetBufferSize()),
		};
		BRK_LOG_ERROR("Failed to compile shader: {}", message);
		logs->Release();
	}
	return byteCode;
}

brk::rdr::VertexShader::VertexShader()
{
	RendererData& data = *Renderer::s_Instance.GetData();
	m_Handle = CreateShader<ID3D11VertexShader>(data.m_Device, data.m_DefaultVShader);
}

brk::rdr::VertexShader::VertexShader(StringView source, const char* entryPoint)
	: m_ByteCode{ d3d::CompileShader(source, "vs_5_0", entryPoint) }
{
	if (!m_ByteCode)
		return;
	ID3D11Device* device = Renderer::s_Instance.GetData()->m_Device;
	m_Handle =
		CreateShader<ID3D11VertexShader>(device, static_cast<ID3DBlob*>(m_ByteCode));
}

brk::rdr::VertexShader::~VertexShader()
{
	if (m_ByteCode)
		static_cast<ID3DBlob*>(m_ByteCode)->Release();
}

void brk::rdr::VertexShader::Reset()
{
	if (m_ByteCode)
	{
		static_cast<ID3DBlob*>(m_ByteCode)->Release();
		m_ByteCode = nullptr;
	}
	if (m_Handle)
	{
		m_Handle->Release();
		m_Handle = nullptr;
	}
}

brk::rdr::FragmentShader::FragmentShader()
{
	RendererData& data = *Renderer::s_Instance.GetData();
	m_Handle = CreateShader<ID3D11PixelShader>(data.m_Device, data.m_DefaultPShader);
}

brk::rdr::FragmentShader::FragmentShader(StringView source, const char* entryPoint)
	: m_ByteCode{ d3d::CompileShader(source, "ps_5_0", entryPoint) }
{
	if (!m_ByteCode)
		return;
	ID3D11Device* device = Renderer::s_Instance.GetData()->m_Device;
	m_Handle =
		CreateShader<ID3D11PixelShader>(device, static_cast<ID3DBlob*>(m_ByteCode));
}

void brk::rdr::FragmentShader::Reset()
{
	if (m_ByteCode)
	{
		static_cast<ID3DBlob*>(m_ByteCode)->Release();
		m_ByteCode = nullptr;
	}
	if (m_Handle)
	{
		m_Handle->Release();
		m_Handle = nullptr;
	}
}

brk::rdr::FragmentShader::~FragmentShader()
{
	if (m_ByteCode)
		static_cast<ID3DBlob*>(m_ByteCode)->Release();
}
