#pragma once

#include "TypeDefs.hpp"
#include <rendering/brk_rendering_export.h>

namespace brk::rdr {
	template <class T>
	struct ResourceHandle;

	class Buffer;
	class FragmentShader;
	class VertexShader;
	class Texture2d;

	template <>
	struct BRK_RENDERING_API ResourceHandle<Buffer>
	{
		using Type = ID3D11Buffer*;
	};

	template <>
	struct BRK_RENDERING_API ResourceHandle<VertexShader>
	{
		using Type = ID3D11VertexShader*;
	};
	template <>
	struct BRK_RENDERING_API ResourceHandle<FragmentShader>
	{
		using Type = ID3D11PixelShader*;
	};
	template <>
	struct BRK_RENDERING_API ResourceHandle<Texture2d>
	{
		struct Type
		{
			ID3D11Texture2D* m_Tex = nullptr;
			ID3D11ShaderResourceView* m_ShaderResource = nullptr;
			ID3D11RenderTargetView* m_RenderTarget = nullptr;
			ID3D11SamplerState* m_Sampler = nullptr;
		};
	};

	template <class T>
	struct ResourceDeleter;

#define DELETER_DECL(Type)                                                               \
	template <>                                                                          \
	struct BRK_RENDERING_API ResourceDeleter<Type*>                                      \
	{                                                                                    \
		void operator()(Type* handle) noexcept;                                          \
	}

	DELETER_DECL(ID3D11Buffer);
	DELETER_DECL(ID3D11DepthStencilState);
	DELETER_DECL(ID3D11DepthStencilView);
	DELETER_DECL(ID3D11Device);
	DELETER_DECL(ID3D11DeviceContext);
	DELETER_DECL(ID3D11PixelShader);
	DELETER_DECL(ID3D11RasterizerState);
	DELETER_DECL(ID3D11RenderTargetView);
	DELETER_DECL(ID3D11ShaderResourceView);
	DELETER_DECL(ID3D11Texture2D);
	DELETER_DECL(ID3D11VertexShader);
	DELETER_DECL(IDXGISwapChain);

#undef DELETER_DECL
} // namespace brk::rdr