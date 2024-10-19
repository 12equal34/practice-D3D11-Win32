#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <array>


namespace Windows
{
namespace D3D11
{
class D3D11Resources
{
public:
	D3D11Resources(HWND outputHwnd);

private:
	static const D3D_FEATURE_LEVEL FeatureLevels[];
	ID3D11Device* _device;
	IDXGISwapChain* _swapChain;
	ID3D11DeviceContext* _immediateDeviceContext;
	D3D_FEATURE_LEVEL _featureLevel;
	HWND _outputHwnd;
};
}
}

