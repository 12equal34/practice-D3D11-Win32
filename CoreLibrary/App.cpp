#include "pch.h"
#include "App.h"

#include "Timer.h"
#include "Window.h"
#include "WindowClass.h"
#include "BaseFramework.h"
#include "D3D11Resources.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <exception>

Windows::App::App(BaseFramework* framework)
	: _framework(framework)
	, _appState(Windows::App::AppState::Init)
	, _windowsMsg()
	, _timer(std::make_unique<Windows::Timer>())
	, _mainWindowClass(std::make_unique<Windows::WindowClass>(L"main_window"))
{
	std::wstring FrameworkTitle = _framework->GetTitle();
	RECT windowRect{};
	POINT LeftTopPoint{400, 400};
	windowRect.left   = LeftTopPoint.x;
	windowRect.top    = LeftTopPoint.y;
	windowRect.right  = LeftTopPoint.x + static_cast<LONG>(_framework->GetWidth());
	windowRect.bottom = LeftTopPoint.y + static_cast<LONG>(_framework->GetHeight());
	_mainWindow = std::make_unique<Windows::Window>(windowRect, FrameworkTitle, _mainWindowClass.get());
	_framework->SetWidth(static_cast<int>(windowRect.right - windowRect.left));
	_framework->SetHeight(static_cast<int>(windowRect.bottom - windowRect.top));
}

Windows::App::~App()
{
}

int Windows::App::Entry()
{
	return static_cast<int>(MessagePump());
}

WPARAM Windows::App::MessagePump()
{
	while (true)
	{
		BOOL bGotMsg = PeekMessageW(&_windowsMsg, NULL, 0, 0, PM_REMOVE);
		
		if (bGotMsg == 0)
		{
			switch (_appState)
			{
			case AppState::Init:
			{
				Init();
				_appState = AppState::Run;
				break;
			}
			case AppState::Run:
			{
				float deltaSeconds = _timer->ElapsedSeconds();
				Run(deltaSeconds);
				break;
			}
			case AppState::Reset:
			{
				Reset();
				_appState = AppState::Run;
				break;
			}
			default:
			{
				assert(false);
				break;
			}
			}
		}
		else if (bGotMsg != -1)
		{
			TranslateMessage(&_windowsMsg);
			DispatchMessageW(&_windowsMsg);

			if (_windowsMsg.message == WM_QUIT)
			{
				break;
			}
		}
		else
		{
			DWORD error = GetLastError();
			std::cerr << "PeekMessage failed with error: " << error << std::endl;
		}
	}
	return _windowsMsg.wParam;
}

void Windows::App::Init()
{
	_framework->Init();
	StartD3D();
}

void Windows::App::Run(float deltaSeconds)
{
	_framework->RunFrame(deltaSeconds);
	if (_framework->ShouldReset())
	{
		_appState = AppState::Reset;
	}

	Render();
}

void Windows::App::Reset()
{
	_framework->Reset();
}

const D3D_FEATURE_LEVEL Windows::App::FeatureLevels[] =
{
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0
};

void Windows::App::StartD3D()
{
	HWND Hwnd = _mainWindow->GetHwnd();
	
#pragma region "D3D11Device와 D3D11ImmediateContext를 생성한다."
	UINT createDeviceFlags = 0;
#if defined(_DEBUG)
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
	swapChainDesc.OutputWindow = Hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT numOfFeatureLevels = _countof(FeatureLevels);
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createDeviceFlags,
		FeatureLevels,
		numOfFeatureLevels,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&_swapChain,
		&_d3d11Device,
		&_featureLevel,
		&_context);
	if (hr == E_INVALIDARG)
	{
		// 만약 Direct3D 11.0 runtime만 가능한 컴퓨터라면 11.0 버전 디바이스를 생성한다.
		hr = D3D11CreateDeviceAndSwapChain(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createDeviceFlags,
			&FeatureLevels[1],
			numOfFeatureLevels - 1,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&_swapChain,
			&_d3d11Device,
			&_featureLevel,
			&_context);
	}
	if (FAILED(hr))
	{
		throw new std::exception("디바이스 및 스왑 체인 생성 실패");
	}
#pragma endregion

#pragma region "스왑체인의 백버퍼에 대한 렌더 타겟 뷰를 생성 후, 백버퍼를 렌더 타겟으로 바인딩한다."
	UINT bufferIndex = 0;
	if (swapChainDesc.SwapEffect == DXGI_SWAP_EFFECT_DISCARD)
	{
		bufferIndex = 0;
	}
	// Get a pointer to the back buffer.
	ID3D11Texture2D* BackBuffer = nullptr;
	hr = _swapChain->GetBuffer(bufferIndex, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
	if (FAILED(hr))
	{
		throw new std::exception("백 버퍼를 스왑체인으로부터 가져오지 못함.");
	}

	// Create a render-target view.
	//D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
	_d3d11Device->CreateRenderTargetView(BackBuffer, nullptr, &_renderTargetView);

	UINT numOfViews = 1;
	_context->OMSetRenderTargets(numOfViews, &_renderTargetView, nullptr);
#pragma endregion

	_d3d11Resources = std::make_unique<Windows::D3D11::D3D11Resources>(_d3d11Device);
	ID3D11Buffer* VertexBuffer = _d3d11Resources->CreateVertexBuffer();
	ID3D11Buffer* IndexBuffer  = _d3d11Resources->CreateIndexBuffer();
	ID3D11Buffer* ContantBuffer = _d3d11Resources->CreateConstantBuffer();

	UINT StartSlot = 0;
	UINT NumOfVertexBuffers = 1;
	UINT Strides = 0;
	UINT Offsets = 0;
	_context->IASetVertexBuffers(StartSlot, NumOfVertexBuffers, &VertexBuffer, &Strides, &Offsets);

	DXGI_FORMAT IndexBufferFormat = DXGI_FORMAT_R32_UINT;
	UINT IndexOffsets = 0;
	_context->IASetIndexBuffer(IndexBuffer, IndexBufferFormat, IndexOffsets);

	UINT StartSlot = 0;
	UINT NumOfCBuffers = 1;
	_context->VSSetConstantBuffers(StartSlot, NumOfCBuffers, &ContantBuffer);

	D3D11_VIEWPORT Viewport{};
	FLOAT ViewportWidth  = (FLOAT)_framework->GetWidth();
	FLOAT ViewportHeight = (FLOAT)_framework->GetHeight();
	Viewport.Width    = ViewportWidth;
	Viewport.Height   = ViewportHeight;
	Viewport.MinDepth = 0.f;
	Viewport.MaxDepth = 1.f;
	Viewport.TopLeftX = 0.f;
	Viewport.TopLeftY = 0.f;
	UINT numOfViewports = 1;
	_context->RSSetViewports(numOfViewports, &Viewport);
}

void Windows::App::EndD3D()
{
	_d3d11Resources.release();
}

void Windows::App::Render()
{
	const float ClearColor[] = {0.f, 0.f, 0.f, 1.f};
	_context->ClearRenderTargetView(_renderTargetView, ClearColor);


	HRESULT hr;
	UINT SyncInterval  = 0;
	UINT PrensentFlags = 0;
	hr = _swapChain->Present(SyncInterval, PrensentFlags);
	if (FAILED(hr))
	{
		throw std::exception("스왑체인 Present 실패");
	}
}