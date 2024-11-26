#include "ResourceHandles.hpp"
#include <d3d11.h>

namespace brk::rdr {
#define DELETER_DEF(Type)                                                                \
	void ResourceDeleter<Type*>::operator()(Type* handle) noexcept                       \
	{                                                                                    \
		handle->Release();                                                               \
	}

	DELETER_DEF(ID3D11Buffer);
	DELETER_DEF(ID3D11DepthStencilState);
	DELETER_DEF(ID3D11DepthStencilView);
	DELETER_DEF(ID3D11Device);
	DELETER_DEF(ID3D11DeviceContext);
	DELETER_DEF(ID3D11PixelShader);
	DELETER_DEF(ID3D11RasterizerState);
	DELETER_DEF(ID3D11RenderTargetView);
	DELETER_DEF(ID3D11Texture2D);
	DELETER_DEF(ID3D11VertexShader);
	DELETER_DEF(IDXGISwapChain);

#undef DELETER_DEF
} // namespace brk::rdr
