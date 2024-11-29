#ifdef BRK_DX11

#include <rendering/Material.hpp>
#include <rendering/Renderer.hpp>
#include <rendering/Shaders.hpp>
#include <rendering/Texture.hpp>
#include <rendering/Vertex.hpp>
#include "Renderer.hpp" // private api
#include "Shaders.hpp"

#include <core/Assert.hpp>
#include <core/LogManager.hpp>
#include <core/DebugBreak.hpp>
#include <core/ULIDFormatter.hpp>

#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_sdl3.h>

#include <SDL3/SDL_Video.h>

#include <array>
#include <system_error>
#include <d3d11.h>

#include <glm/ext/matrix_clip_space.hpp>

#include "ObjectRef.hpp"

#include <comdef.h>

namespace {
	HWND GetNativeWindowHandle(SDL_Window& window)
	{
		return static_cast<HWND>(SDL_GetPointerProperty(
			SDL_GetWindowProperties(&window),
			SDL_PROP_WINDOW_WIN32_HWND_POINTER,
			nullptr));
	}

	bool LogError(HRESULT err, const char* format)
	{
		DEBUG_CHECK(!err)
		{
			BRK_LOG_CRITICAL(format, _com_error(err).ErrorMessage());
			return true;
		}
		return false;
	}

	ID3D11RenderTargetView* CreateFrameBufferView(
		ID3D11Device* device,
		IDXGISwapChain* swapChain)
	{
		ID3D11Texture2D* frameBuf = nullptr;

		DEBUG_CHECK(!LogError(
			swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&frameBuf),
			"Couldn't get back buffer: {}"))
		{
			return nullptr;
		}

		ID3D11RenderTargetView* view = nullptr;
		LogError(
			device->CreateRenderTargetView(frameBuf, nullptr, &view),
			"Couldn't create frame buffer view: {}");

		frameBuf->Release();

		return view;
	}

	ID3D11DepthStencilView* CreateDepthStencilView(
		ID3D11Device* device,
		ID3D11Texture2D* buffer)
	{
		ID3D11DepthStencilView* view = nullptr;
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Flags = 0;

		LogError(
			device->CreateDepthStencilView(buffer, &desc, &view),
			"Couldn't create frame buffer view: {}");
		return view;
	}

	ID3D11DepthStencilState* CreateDepthStencilState(
		ID3D11Device* device,
		bool enableDepth,
		bool enableStencil)
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = enableDepth;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS;

		desc.StencilEnable = enableStencil;
		desc.StencilReadMask = 0xFF;
		desc.StencilWriteMask = 0xFF;

		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		ID3D11DepthStencilState* ptr = nullptr;
		LogError(
			device->CreateDepthStencilState(&desc, &ptr),
			"Couldn't create depth stencil state: {}");
		return ptr;
	}

	ID3D11InputLayout* CreateInputLayout(ID3D11Device* device, ID3DBlob* vShaderCode)
	{
		constexpr D3D11_INPUT_ELEMENT_DESC elements[] = {
			{
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0,
			},
			{
				"NORMAL",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0,
			},
			{
				"TEXCOORD",
				0,
				DXGI_FORMAT_R32G32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0,
			},
		};
		ID3D11InputLayout* ptr = nullptr;
		LogError(
			device->CreateInputLayout(
				elements,
				std::extent_v<decltype(elements)>,
				vShaderCode->GetBufferPointer(),
				vShaderCode->GetBufferSize(),
				&ptr),
			"Failed to create input layout: {}");
		return ptr;
	}

	ID3D11RasterizerState* CreateRasterizer(
		ID3D11Device* device,
		const D3D11_RASTERIZER_DESC& desc)
	{
		ID3D11RasterizerState* ptr = nullptr;
		LogError(
			device->CreateRasterizerState(&desc, &ptr),
			"Failed to create rasterizer state: {}");
		return ptr;
	}

	void UpdateShaderStages(
		ID3D11DeviceContext* context,
		const brk::rdr::RendererData::PipelineState& state)
	{
		constexpr uint32 numTex = std::extent_v<decltype(state.m_ShaderResources)>;

		context->VSSetShader(state.m_VertexShader, nullptr, 0);
		context->VSSetConstantBuffers(0, 3, &state.m_ParamBuffer);
		context->VSSetShaderResources(0, numTex, state.m_ShaderResources);
		context->VSSetSamplers(0, numTex, state.m_Samplers);

		context->PSSetShader(state.m_PixelShader, nullptr, 0);
		context->PSSetConstantBuffers(0, 3, &state.m_ParamBuffer);
		context->PSSetShaderResources(0, numTex, state.m_ShaderResources);
		context->PSSetSamplers(0, numTex, state.m_Samplers);
	}

	constexpr std::array s_Dx11FeatureLevels = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
} // namespace

brk::rdr::RendererData::RendererData(SDL_Window& window)
	: m_NativeWindow{ GetNativeWindowHandle(window) }
{
	int32 width, height;
	SDL_GetWindowSize(&window, &width, &height);

	UINT deviceFlags = 0;
#ifdef BRK_DEV
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_NativeWindow;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.RefreshRate = { 0, 1 };
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	swapChainDesc.BufferCount = 2;
	swapChainDesc.Windowed = true;

	HRESULT result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		deviceFlags,
		s_Dx11FeatureLevels.data(),
		UINT(s_Dx11FeatureLevels.size()),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_SwapChain.m_Handle,
		&m_Device.m_Handle,
		nullptr,
		&m_DeviceContext.m_Handle);

	DEBUG_CHECK(!LogError(result, "Couldn't initialize D3D11 swap chain: {}"))
	{
		dbg::Break();
	}

	m_FrameBufferView.m_Handle = CreateFrameBufferView(m_Device, m_SwapChain);
	m_DepthStencilBuffer.m_Handle = CreateTexture2d(CD3D11_TEXTURE2D_DESC{
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		uint32(width),
		uint32(height),
		1,
		0,
		D3D11_BIND_DEPTH_STENCIL,
		D3D11_USAGE_DEFAULT,
	});

	DEBUG_CHECK(m_FrameBufferView && m_DepthStencilBuffer)
	{
		dbg::Break();
	}

	m_DepthStencilView.m_Handle = CreateDepthStencilView(m_Device, m_DepthStencilBuffer);

	DEBUG_CHECK(m_DepthStencilView)
	{
		dbg::Break();
	}

	if (SDL_GetWindowFlags(&window) & SDL_WINDOW_FULLSCREEN)
	{
		m_SwapChain->SetFullscreenState(true, nullptr);
	}

	m_DepthStencilState.m_Handle = CreateDepthStencilState(m_Device, true, true);
	DEBUG_CHECK(m_DepthStencilState)
	{
		dbg::Break();
	}
	{
		CD3D11_RASTERIZER_DESC rasterizerDesc{ D3D11_DEFAULT };
		rasterizerDesc.FrontCounterClockwise = true;
		m_DefaultRasterizer = CreateRasterizer(m_Device, rasterizerDesc);
	}
	DEBUG_CHECK(m_DefaultRasterizer)
	{
		dbg::Break();
	}
	m_DeviceContext->RSSetState(m_DefaultRasterizer);

	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
#ifdef BRK_DEV
	ImGui_ImplSDL3_InitForD3D(&window);
	ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
#endif

	m_DefaultVShader = d3d::CompileShader(s_DefaultShaderCode, "vs_5_0", "vs_main");
	m_DefaultPShader = d3d::CompileShader(s_DefaultShaderCode, "ps_5_0", "fs_main");
	BRK_ASSERT(m_DefaultVShader && m_DefaultPShader, "Failed to compile default shaders");
	m_InputLayout = CreateInputLayout(m_Device, m_DefaultVShader);
	DEBUG_CHECK(m_InputLayout)
	{
		dbg::Break();
	}
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	{
		constexpr auto defaultTransform = glm::identity<glm::mat4x4>();
		const float ratio = float(width) / float(height);
		const auto proj = glm::orthoRH(-ratio, ratio, -1.f, 1.0f, 0.01f, 100.f);
		m_FrameDataBuffer = CreateBuffer(
			CD3D11_BUFFER_DESC{
				sizeof(proj),
				D3D11_BIND_CONSTANT_BUFFER,
				D3D11_USAGE_DYNAMIC,
				D3D11_CPU_ACCESS_WRITE,
			},
			&proj);
		m_TransformDataBuffer = CreateBuffer(
			CD3D11_BUFFER_DESC{
				sizeof(defaultTransform),
				D3D11_BIND_CONSTANT_BUFFER,
				D3D11_USAGE_DYNAMIC,
				D3D11_CPU_ACCESS_WRITE,
			},
			&defaultTransform);
	}
	m_CurrentPipelineState.m_FrameData = m_FrameDataBuffer;
	m_CurrentPipelineState.m_TransformData = m_TransformDataBuffer;
}

ID3D11Buffer* brk::rdr::RendererData::CreateBuffer(
	const D3D11_BUFFER_DESC& desc,
	const void* data)
{
	ID3D11Buffer* ptr = nullptr;
	const D3D11_SUBRESOURCE_DATA subRes{ data, desc.ByteWidth };
	LogError(
		m_Device->CreateBuffer(&desc, data ? &subRes : nullptr, &ptr),
		"Failed to create buffer: {}");
	return ptr;
}

ID3D11Texture2D* brk::rdr::RendererData::CreateTexture2d(
	const D3D11_TEXTURE2D_DESC& desc,
	const void* data,
	uint32 pitch)
{
	ID3D11Texture2D* res = nullptr;
	const D3D11_SUBRESOURCE_DATA dataDesc{ data, pitch, pitch * desc.Height };
	LogError(
		m_Device->CreateTexture2D(&desc, data ? &dataDesc : nullptr, &res),
		"Failed to create Texture2d: {}");
	return res;
}

ID3D11ShaderResourceView* brk::rdr::RendererData::CreateShaderResourceView(
	ID3D11Resource& res,
	const D3D11_SHADER_RESOURCE_VIEW_DESC& desc)
{
	ID3D11ShaderResourceView* ptr = nullptr;
	LogError(
		m_Device->CreateShaderResourceView(&res, &desc, &ptr),
		"Failed to create shader resource view: {}");
	return ptr;
}

ID3D11RenderTargetView* brk::rdr::RendererData::CreateRenderTargetView(
	ID3D11Resource& res,
	const D3D11_RENDER_TARGET_VIEW_DESC& desc)
{
	ID3D11RenderTargetView* ptr = nullptr;
	LogError(
		m_Device->CreateRenderTargetView(&res, &desc, &ptr),
		"Failed to create render target view: {}");
	return ptr;
}

ID3D11SamplerState* brk::rdr::RendererData::CreateSamplerState(
	const D3D11_SAMPLER_DESC& desc)
{
	ID3D11SamplerState* ptr = nullptr;
	LogError(m_Device->CreateSamplerState(&desc, &ptr), "Failed to create sampler: {}");
	return ptr;
}

brk::rdr::RendererData::~RendererData() = default;

void brk::rdr::Renderer::Init(SDL_Window* window)
{
	m_Window = window;
	BRK_ASSERT(m_Window, "Trying to initialize renderer with invalid window pointer!");

	m_Data = new RendererData{ *window };
}

#ifdef BRK_DEV
void brk::rdr::Renderer::NewImGuiFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}
#endif

void brk::rdr::Renderer::ResizeFrameBuffers(uint32 width, uint32 height)
{
	m_Data->m_FrameBufferView.Reset();
	m_Data->m_DepthStencilView.Reset();

	m_Data->m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

	m_Data->m_DepthStencilBuffer.Reset(m_Data->CreateTexture2d(CD3D11_TEXTURE2D_DESC{
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		width,
		height,
		1,
		0,
		D3D11_BIND_DEPTH_STENCIL,
		D3D11_USAGE_DEFAULT,
	}));

	m_Data->m_FrameBufferView.Reset(
		CreateFrameBufferView(m_Data->m_Device, m_Data->m_SwapChain));
	m_Data->m_DepthStencilView.Reset(
		CreateDepthStencilView(m_Data->m_Device, m_Data->m_DepthStencilBuffer));
}

void brk::rdr::Renderer::StartRender()
{
	m_Data->m_DeviceContext->ClearRenderTargetView(
		m_Data->m_FrameBufferView,
		(float*)&m_ClearColor);
	m_Data->m_DeviceContext->ClearDepthStencilView(
		m_Data->m_DepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
	m_Data->m_DeviceContext->OMSetRenderTargets(
		1,
		&m_Data->m_FrameBufferView.m_Handle,
		m_Data->m_DepthStencilView);

	RECT winRect;
	GetClientRect(m_Data->m_NativeWindow, &winRect);
	D3D11_VIEWPORT viewport = {};
	viewport.Width = float(winRect.right - winRect.left);
	viewport.Height = float(winRect.bottom - winRect.top);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_Data->m_DeviceContext->RSSetViewports(1, &viewport);
	m_Data->m_DeviceContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

#ifdef BRK_DEV
	ImGui::Render();
#endif
}

void brk::rdr::Renderer::SetMaterial(const MaterialInstance& material)
{
	auto& pipelineState = m_Data->m_CurrentPipelineState;

	pipelineState.m_VertexShader = material.GetVertexShader().GetHandle();
	BRK_ASSERT(
		pipelineState.m_VertexShader,
		"Tried to bind material {} to the pipeline, which has an invalid vertex shader",
		material.GetName());
	const auto* textures = material.GetTextures();
	for (uint32 i = 0; i < MaterialInstance::s_MaxTextureCount; i++)
	{
		if (!textures[i])
		{
			pipelineState.m_Samplers[i] = nullptr;
			pipelineState.m_ShaderResources[i] = nullptr;
			continue;
		}
		const auto& handle = textures[i]->GetHandle();
		BRK_ASSERT(
			handle.m_ShaderResource && handle.m_Sampler,
			"Tried to bind texture {} ({}) to render pipeline, but texture isn't a "
			"shader resource",
			textures[i]->GetName(),
			textures[i]->GetId());
		pipelineState.m_Samplers[i] = handle.m_Sampler;
		pipelineState.m_ShaderResources[i] = handle.m_ShaderResource;
	}

	pipelineState.m_PixelShader = material.GetFragmentShader().GetHandle();
	BRK_ASSERT(
		pipelineState.m_PixelShader,
		"Tried to bind material {} to the pipeline, which has an invalid fragment shader",
		material.GetName());

	pipelineState.m_ParamBuffer = material.GetParamBuffer().GetHandle();

	UpdateShaderStages(m_Data->m_DeviceContext, pipelineState);
}

void brk::rdr::Renderer::DrawIndexed(
	const Buffer& vBuf,
	const Buffer& iBuf,
	uint32 nIndices)
{
	ID3D11Buffer* vBufPtr = vBuf.GetHandle();
	constexpr uint32 stride = sizeof(brk::rdr::Vertex3d);
	const uint32 offset = 0;
	auto& pipelineState = m_Data->m_CurrentPipelineState;

	m_Data->m_DeviceContext->IASetVertexBuffers(0, 1, &vBufPtr, &stride, &offset);
	m_Data->m_DeviceContext->IASetIndexBuffer(iBuf.GetHandle(), DXGI_FORMAT_R32_UINT, 0);
	m_Data->m_DeviceContext->DrawIndexed(nIndices, 0, 0);
}

void brk::rdr::Renderer::RenderUI()
{
#ifdef BRK_DEV
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
}

void brk::rdr::Renderer::Shutdown()
{
	if (!m_Data)
		return;

#ifdef BRK_DEV
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
#endif

	delete m_Data;
	m_Data = nullptr;
}

void brk::rdr::Renderer::Present()
{
	m_Data->m_SwapChain->Present(0, 0);
}

#endif