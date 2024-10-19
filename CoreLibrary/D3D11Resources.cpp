#include "pch.h"
#include "D3D11Resources.h"

#include <exception>

#define ThrowIfFailed(x) if ( FAILED((x)) ) { throw new std::exception{}; }

const D3D_FEATURE_LEVEL Windows::D3D11::D3D11Resources::FeatureLevels[] 
	= {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0};

Windows::D3D11::D3D11Resources::D3D11Resources(HWND outputHwnd)
	: _device(nullptr)
	, _outputHwnd(outputHwnd)
{
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = 640;
	swapChainDesc.BufferDesc.Height = 640;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = _outputHwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, createDeviceFlags,
		FeatureLevels, _countof(FeatureLevels),
		D3D11_SDK_VERSION,
		&swapChainDesc, &_swapChain,
		&_device, &_featureLevel, &_immediateDeviceContext));
}

