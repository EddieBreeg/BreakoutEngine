#pragma once

struct ID3D11Buffer;
struct ID3D10Blob;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3D11PixelShader;
struct ID3D11RasterizerState;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11VertexShader;
struct IDXGISwapChain;

struct D3D11_BUFFER_DESC;

using ID3DBlob = ID3D10Blob;

using HWND = struct HWND__*;