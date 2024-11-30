#pragma once

#ifdef BRK_DX11

#include <PCH.hpp>
#include "ObjectRef.hpp"
#include "TypeDefs.hpp"

struct SDL_Window;

namespace brk::rdr {
	struct RendererData
	{
		RendererData(SDL_Window& window);

		d3d::Ref<ID3D11Device> m_Device;
		d3d::Ref<ID3D11DeviceContext> m_DeviceContext;
		d3d::Ref<IDXGISwapChain> m_SwapChain;
		d3d::Ref<ID3D11RenderTargetView> m_FrameBufferView;

		d3d::Ref<ID3D11Texture2D> m_DepthStencilBuffer;
		d3d::Ref<ID3D11DepthStencilView> m_DepthStencilView;
		d3d::Ref<ID3D11DepthStencilState> m_DepthStencilState;
		d3d::Ref<ID3D11RasterizerState> m_DefaultRasterizer;

		d3d::Ref<ID3DBlob> m_DefaultVShader, m_DefaultPShader;
		d3d::Ref<ID3D11InputLayout> m_InputLayout;
		d3d::Ref<ID3D11Buffer> m_FrameDataBuffer, m_TransformDataBuffer;

		ID3D11Buffer* CreateBuffer(
			const D3D11_BUFFER_DESC& desc,
			const void* data = nullptr);
		ID3D11Texture2D* CreateTexture2d(
			const D3D11_TEXTURE2D_DESC& desc,
			const void* data = nullptr,
			uint32 pitch = 0);

		ID3D11ShaderResourceView* CreateShaderResourceView(
			ID3D11Resource& res,
			const D3D11_SHADER_RESOURCE_VIEW_DESC& desc);
		ID3D11RenderTargetView* CreateRenderTargetView(
			ID3D11Resource& res,
			const D3D11_RENDER_TARGET_VIEW_DESC& desc);
		ID3D11SamplerState* CreateSamplerState(const D3D11_SAMPLER_DESC& desc);

		void UpdateDynamicResource(
			ID3D11Resource& res,
			uint32 subRes,
			const void* data,
			uint32 dataSize);

		struct PipelineState
		{
			ID3D11Buffer* m_ParamBuffer = nullptr;
			ID3D11Buffer* m_FrameData = nullptr;
			ID3D11Buffer* m_TransformData = nullptr;

			ID3D11VertexShader* m_VertexShader = nullptr;
			ID3D11PixelShader* m_PixelShader = nullptr;

			ID3D11ShaderResourceView* m_ShaderResources[8] = { nullptr };
			ID3D11SamplerState* m_Samplers[8] = { nullptr };

			[[nodiscard]] operator bool() const noexcept
			{
				return m_VertexShader && m_PixelShader && m_FrameData && m_TransformData;
			}
		} m_CurrentPipelineState;

		HWND m_NativeWindow = nullptr;

		~RendererData();
	};
} // namespace brk::rdr

#endif