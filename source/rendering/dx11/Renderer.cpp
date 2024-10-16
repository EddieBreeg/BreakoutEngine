#ifdef BRK_DX11

#include <rendering/Renderer.hpp>

#include <core/Assert.hpp>
#include <core/LogManager.hpp>
#include <core/DebugBreak.hpp>

#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_sdl2.h>

#include <SDL2/SDL_syswm.h>

#include <array>
#include <system_error>
#include <d3d11.h>

#include "ObjectRef.hpp"

struct brk::rdr::RendererData
{
	d3d::ObjectRef<ID3D11Device> m_Device;
	d3d::ObjectRef<ID3D11DeviceContext> m_DeviceContext;
	d3d::ObjectRef<IDXGISwapChain> m_SwapChain;
	d3d::ObjectRef<ID3D11RenderTargetView> m_FrameBufferView;
	HWND m_NativeWindow = nullptr;
};

namespace {
	HWND GetNativeWindowHandle(SDL_Window* window)
	{
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		SDL_GetWindowWMInfo(window, &info);
		return info.info.win.window;
	}

	ID3D11RenderTargetView* CreateFrameBufferView(
		ID3D11Device* device,
		IDXGISwapChain* swapChain)
	{
		ID3D11Texture2D* frameBuf = nullptr;
		HRESULT res =
			swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&frameBuf);

		DEBUG_CHECK(res == S_OK)
		{
			goto VIEW_ERROR;
		}

		ID3D11RenderTargetView* view = nullptr;
		res = device->CreateRenderTargetView(frameBuf, nullptr, &view);
		frameBuf->Release();

		DEBUG_CHECK(res == S_OK)
		{
			goto VIEW_ERROR;
		}
		return view;

	VIEW_ERROR:
		BRK_LOG_CRITICAL(
			"Couldn't create frame buffer view: {}",
			std::system_category().message(res));
		return nullptr;
	}

	constexpr std::array s_Dx11FeatureLevels = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
} // namespace

void brk::rdr::Renderer::Init(SDL_Window* window)
{
	m_Window = window;
	BRK_ASSERT(m_Window, "Trying to initialize renderer with invalid window pointer!");
	HWND const nativeWinHandle = GetNativeWindowHandle(window);

	UINT deviceFlags = 0;
#ifdef BRK_DEV
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = nativeWinHandle;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.RefreshRate = { 0, 1 };
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	swapChainDesc.BufferCount = 2;
	swapChainDesc.Windowed = true;

	ID3D11Device* dev = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11DeviceContext* devContext = nullptr;

	HRESULT result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		deviceFlags,
		s_Dx11FeatureLevels.data(),
		UINT(s_Dx11FeatureLevels.size()),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&dev,
		nullptr,
		&devContext);

	DEBUG_CHECK(!result)
	{
		std::string msg = std::system_category().message(result);
		BRK_LOG_CRITICAL("Couldn't initialize D3D11 swap chain: {}", msg);
		dbg::Break();
	}

	ID3D11RenderTargetView* frameBufferView = CreateFrameBufferView(dev, swapChain);
	DEBUG_CHECK(frameBufferView)
	{
		dbg::Break();
	}

	if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN)
	{
		swapChain->SetFullscreenState(true, nullptr);
	}

	m_Data = new RendererData{
		d3d::ObjectRef<ID3D11Device>{ dev },
		d3d::ObjectRef<ID3D11DeviceContext>{ devContext },
		d3d::ObjectRef<IDXGISwapChain>{ swapChain },
		d3d::ObjectRef<ID3D11RenderTargetView>{ frameBufferView },
		nativeWinHandle,
	};

#ifdef BRK_DEV
	ImGui_ImplSDL2_InitForD3D(window);
	ImGui_ImplDX11_Init(dev, devContext);
#endif
}

#ifdef BRK_DEV
void brk::rdr::Renderer::NewImGuiFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}
#endif

void brk::rdr::Renderer::ResizeFrameBuffers()
{
	m_Data->m_FrameBufferView.reset();
	m_Data->m_SwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	m_Data->m_FrameBufferView.reset(
		CreateFrameBufferView(m_Data->m_Device.get(), m_Data->m_SwapChain.get()));
}

void brk::rdr::Renderer::StartRender()
{
	m_Data->m_DeviceContext->ClearRenderTargetView(
		m_Data->m_FrameBufferView.get(),
		(float*)&m_ClearColor);
	ID3D11RenderTargetView* targetViewPtr = m_Data->m_FrameBufferView.get();
	m_Data->m_DeviceContext->OMSetRenderTargets(1, &targetViewPtr, nullptr);

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
	ImGui_ImplSDL2_Shutdown();
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