#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <array>
#include <vector>

namespace Windows
{
namespace D3D11
{
class D3D11Resources
{
public:
	D3D11Resources(ID3D11Device* Device);
	~D3D11Resources();

public:
	// 연결된 모든 GPU 어뎁터들을 구한다.
	std::vector<IDXGIAdapter*> EnumerateAdapters() const;
	// GPU와 연결된 모든 출력 모니터를 구한다.
	std::vector<IDXGIOutput*> GetOutputs(IDXGIAdapter* Adapter) const;
	// 출력 모니터로부터 가능한 모든 디스플레이 모드를 구한다.
	std::vector<DXGI_MODE_DESC> GetDisplayModes(IDXGIOutput* Output, DXGI_FORMAT Format) const;

public:
	ID3D11Buffer* CreateVertexBuffer();
	ID3D11Buffer* CreateIndexBuffer();
	ID3D11Buffer* CreateConstantBuffer();

	void CreateTexture();
private:
	IDXGIFactory* _factory;
	ID3D11Device* _device;

	ID3D11Texture2D* _texture;
};
}
}

