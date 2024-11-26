#pragma once

#ifdef BRK_DX11

#include <PCH.hpp>
#include "ObjectRef.hpp"
#include "TypeDefs.hpp"

struct SDL_Window;

namespace brk::rdr {
	struct brk::rdr::RendererData
	{
		RendererData(SDL_Window& window);

		d3d::Ref<ID3D11Device> m_Device;
		d3d::Ref<ID3D11DeviceContext> m_DeviceContext;
		d3d::Ref<IDXGISwapChain> m_SwapChain;
		d3d::Ref<ID3D11RenderTargetView> m_FrameBufferView;

		d3d::Ref<ID3D11Texture2D> m_DepthStencilBuffer;
		d3d::Ref<ID3D11DepthStencilView> m_DepthStencilView;
		d3d::Ref<ID3D11DepthStencilState> m_DepthStencilState;

		d3d::Ref<ID3DBlob> m_DefaultVShader, m_DefaultPShader;

		HWND m_NativeWindow = nullptr;

		~RendererData();
	};
} // namespace brk::rdr

#endif