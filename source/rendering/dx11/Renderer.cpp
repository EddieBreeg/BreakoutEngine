#ifdef BRK_DX11

#include <rendering/Renderer.hpp>
#include "Renderer.hpp" // private api

#include <core/Assert.hpp>
#include <core/LogManager.hpp>
#include <core/DebugBreak.hpp>

#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_sdl3.h>

#include <SDL3/SDL_Video.h>

#include <array>
#include <system_error>
#include <d3d11.h>

#include "ObjectRef.hpp"

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
			BRK_LOG_CRITICAL(format, std::strerror(errno));
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

	ID3D11Texture2D* CreateTexture2d(
		ID3D11Device* dev,
		uint32 width,
		uint32 height,
		DXGI_FORMAT format,
		D3D11_USAGE usage,
		D3D11_BIND_FLAG bindFlags,
		D3D11_CPU_ACCESS_FLAG cpuAccess)
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;
		desc.Usage = usage;
		desc.BindFlags = bindFlags;
		desc.CPUAccessFlags = cpuAccess;
		desc.SampleDesc = { 1, 0 };
		ID3D11Texture2D* ptr = nullptr;
		const HRESULT err = dev->CreateTexture2D(&desc, nullptr, &ptr);
		LogError(err, "Couldn't create 2D texture: {}");
		return ptr;
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
	m_DepthStencilBuffer.m_Handle = CreateTexture2d(
		m_Device,
		width,
		height,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_FLAG(D3D11_BIND_DEPTH_STENCIL),
		D3D11_CPU_ACCESS_FLAG(0));

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

	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
#ifdef BRK_DEV
	ImGui_ImplSDL3_InitForD3D(&window);
	ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
#endif
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

	m_Data->m_DepthStencilBuffer.Reset(CreateTexture2d(
		m_Data->m_Device,
		width,
		height,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_DEPTH_STENCIL,
		D3D11_CPU_ACCESS_FLAG(0)));

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
	m_Data->m_DeviceContext->OMSetRenderTargets(
		1,
		&m_Data->m_FrameBufferView.m_Handle,
		m_Data->m_DepthStencilView);

	RECT winRect;
	GetClientRect(m_Data->m_NativeWindow, &winRect);
	D3D11_VIEWPORT viewport = {};
	viewport.Width = float(winRect.right - winRect.left);
	viewport.Height = float(winRect.bottom - winRect.top);
	m_Data->m_DeviceContext->RSSetViewports(1, &viewport);

#ifdef BRK_DEV
	ImGui::Render();
#endif
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