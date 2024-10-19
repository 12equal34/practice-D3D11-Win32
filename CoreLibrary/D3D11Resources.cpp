#include "pch.h"
#include "D3D11Resources.h"

#include <DirectXMath.h>
#include <exception>

Windows::D3D11::D3D11Resources::D3D11Resources(ID3D11Device* Device)
	: _device(Device)
	, _factory(nullptr)
{
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&_factory);
	if (FAILED(hr))
	{
		throw new std::exception("DXGIFactory 생성 실패");
	}
}

Windows::D3D11::D3D11Resources::~D3D11Resources()
{
}

std::vector<IDXGIAdapter*> Windows::D3D11::D3D11Resources::EnumerateAdapters() const
{
	std::vector<IDXGIAdapter*> Adapters;
	IDXGIAdapter* Adapter = nullptr;
	for (UINT AdapterIndex = 0; _factory->EnumAdapters(AdapterIndex, &Adapter) != DXGI_ERROR_NOT_FOUND; ++AdapterIndex)
	{
		Adapters.push_back(Adapter);
	}
	return Adapters;
}

std::vector<IDXGIOutput*> Windows::D3D11::D3D11Resources::GetOutputs(IDXGIAdapter* Adapter) const
{
	std::vector<IDXGIOutput*> Outputs;
	IDXGIOutput* Output = nullptr;
	for (UINT OutputIndex = 0; Adapter->EnumOutputs(OutputIndex, &Output) != DXGI_ERROR_NOT_FOUND; ++OutputIndex)
	{
		Outputs.push_back(Output);
	}
	return Outputs;
}

std::vector<DXGI_MODE_DESC> Windows::D3D11::D3D11Resources::GetDisplayModes(IDXGIOutput* Output, DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT) const
{
	HRESULT hr;
	UINT DisplayModeFlags = 0;

	// 출력모드들의 갯수를 구한다.
	UINT NumOfModes = 0;
	hr = Output->GetDisplayModeList(Format, DisplayModeFlags, &NumOfModes, nullptr);
	if (FAILED(hr))
	{
		throw std::exception();
	}

	// 출력모드들을 구한다.
	DXGI_MODE_DESC* DisplayModes = new DXGI_MODE_DESC[NumOfModes];
	hr = Output->GetDisplayModeList(Format, DisplayModeFlags, &NumOfModes, DisplayModes);
	if (FAILED(hr))
	{
		throw std::exception();
	}

	std::vector<DXGI_MODE_DESC> Result(NumOfModes);
	for (int i = 0; i < NumOfModes; ++i)
	{
		Result.push_back(DisplayModes[i]);
	}
	delete[] DisplayModes;
	DisplayModes = nullptr;
	return Result;
}

ID3D11Buffer* Windows::D3D11::D3D11Resources::CreateVertexBuffer()
{
	ID3D11Buffer* VertexBuffer = nullptr;
	using DirectX::XMFLOAT3;

	struct SimpleVertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Col;
	};
	
	std::vector<SimpleVertex> Vertices
	{
		{
			XMFLOAT3(0.0f, 0.5f, 0.5f),
			XMFLOAT3(0.0f, 0.0f, 0.5f),
		},
		{
			XMFLOAT3(0.5f, -0.5f, 0.5f),
			XMFLOAT3(0.5f, 0.0f, 0.0f),
		},
		{
			XMFLOAT3(-0.5f, -0.5f, 0.5f),
			XMFLOAT3(0.0f, 0.5f, 0.0f),
		},
	};

	D3D11_BUFFER_DESC BufferDesc{};
	BufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	BufferDesc.ByteWidth      = sizeof(SimpleVertex) * Vertices.size();
	BufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags      = 0;

	D3D11_SUBRESOURCE_DATA InitData{};
	InitData.pSysMem = (const void*) Vertices.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr = _device->CreateBuffer(&BufferDesc, &InitData, &VertexBuffer);
	if (FAILED(hr))
	{
		throw std::exception("버텍스 버퍼 생성 실패");
	}
	return VertexBuffer;

}

ID3D11Buffer* Windows::D3D11::D3D11Resources::CreateIndexBuffer()
{
	ID3D11Buffer* IndexBuffer = nullptr;
	std::vector<UINT> Indices = {0,1,2};

	D3D11_BUFFER_DESC BufferDesc{};
	BufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	BufferDesc.ByteWidth      = sizeof(UINT) * Indices.size();
	BufferDesc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags      = 0;

	D3D11_SUBRESOURCE_DATA InitData{};
	InitData.pSysMem = (const void*) Indices.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr = _device->CreateBuffer(&BufferDesc, &InitData, &IndexBuffer);
	if (FAILED(hr))
	{
		throw std::exception("인덱스 버퍼 생성 실패");
	}
	return IndexBuffer;
}

ID3D11Buffer* Windows::D3D11::D3D11Resources::CreateConstantBuffer()
{
	ID3D11Buffer* CBuffer = nullptr;
	using DirectX::XMFLOAT4;
	using DirectX::XMFLOAT4X4;

	struct VS_CONSTANT_BUFFER
	{
		XMFLOAT4X4 mWorldViewProj;
		XMFLOAT4 vSomeVectorThatMayBeNeededByASpecificShader;
		float fSomeFloatThatMayBeNeededByASpecificShader;
		float fTime;
		float fSomeFloatThatMayBeNeededByASpecificShader2;
		float fSomeFloatThatMayBeNeededByASpecificShader3;
	};

	VS_CONSTANT_BUFFER ConstantBufferData{};
	//ConstantBufferData.mWorldViewProj = {...};
	ConstantBufferData.vSomeVectorThatMayBeNeededByASpecificShader = XMFLOAT4(1, 2, 3, 4);
	ConstantBufferData.fSomeFloatThatMayBeNeededByASpecificShader = 3.0f;
	ConstantBufferData.fTime = 1.0f;
	ConstantBufferData.fSomeFloatThatMayBeNeededByASpecificShader2 = 2.0f;
	ConstantBufferData.fSomeFloatThatMayBeNeededByASpecificShader3 = 4.0f;

	D3D11_BUFFER_DESC BufferDesc{};
	BufferDesc.ByteWidth      = sizeof(VS_CONSTANT_BUFFER);
	BufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
	BufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags      = 0;
	BufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData{};
	InitData.pSysMem = &ConstantBufferData;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr = _device->CreateBuffer(&BufferDesc, &InitData, &CBuffer);
	if (FAILED(hr))
	{
		throw std::exception("인덱스 버퍼 생성 실패");
	}
	return CBuffer;
}

void Windows::D3D11::D3D11Resources::CreateTexture()
{
	D3D11_TEXTURE2D_DESC TextureDesc{};
	TextureDesc.Width            = 256;
	TextureDesc.Height           = 256;
	TextureDesc.MipLevels        = TextureDesc.ArraySize = 1;
	TextureDesc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage            = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags   = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags        = 0;

	HRESULT hr;
	hr = _device->CreateTexture2D(&TextureDesc, NULL, &_texture);
	if (FAILED(hr))
	{
		throw std::exception("텍스쳐 생성 실패");
	}
}