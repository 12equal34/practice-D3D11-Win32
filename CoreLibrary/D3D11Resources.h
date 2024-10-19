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
	// ����� ��� GPU ��͵��� ���Ѵ�.
	std::vector<IDXGIAdapter*> EnumerateAdapters() const;
	// GPU�� ����� ��� ��� ����͸� ���Ѵ�.
	std::vector<IDXGIOutput*> GetOutputs(IDXGIAdapter* Adapter) const;
	// ��� ����ͷκ��� ������ ��� ���÷��� ��带 ���Ѵ�.
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

