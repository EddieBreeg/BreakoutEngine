#pragma once

#include "TypeDefs.hpp"

namespace brk::rdr {
	template <class T>
	struct ResourceHandle;

	class Buffer;
	class FragmentShader;
	class VertexShader;

	template <>
	struct ResourceHandle<Buffer>
	{
		using Type = ID3D11Buffer*;
	};

	template <>
	struct ResourceHandle<VertexShader>
	{
		using Type = ID3D11VertexShader*;
	};
	template <>
	struct ResourceHandle<FragmentShader>
	{
		using Type = ID3D11PixelShader*;
	};

	template <class T>
	struct ResourceDeleter;

#define DELETER_DECL(Type)                                                               \
	template <>                                                                          \
	struct ResourceDeleter<Type*>                                                         \
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
	DELETER_DECL(ID3D11Texture2D);
	DELETER_DECL(ID3D11VertexShader);
	DELETER_DECL(IDXGISwapChain);

#undef DELETER_DECL
} // namespace brk::rdr
