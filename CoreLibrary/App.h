#pragma once

#include "Windows.h"
#include <memory>
#include <exception>
#include <iostream>

class BaseFramework;

namespace Windows
{
class Timer;
class Window;
class WindowClass;

namespace D3D11
{
class D3D11Resources;
}

class App
{
public:
	App(BaseFramework* framework);
	~App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;
public:
	enum class AppState { Init, Run, Reset };
public:
	int Entry();
	WPARAM MessagePump();
	void Run(float deltaSeconds);
	void Init();
	void Reset();
private:
	std::unique_ptr<WindowClass> _mainWindowClass;
private:
	AppState _appState;
	MSG _windowsMsg;
	std::unique_ptr<Timer> _timer;
	std::unique_ptr<Window> _mainWindow;
private:
	BaseFramework* _framework;
private:
	void StartD3D();
	void EndD3D();
	void Render();
private:
	static const D3D_FEATURE_LEVEL FeatureLevels[];
	D3D_FEATURE_LEVEL _featureLevel;
	std::unique_ptr<D3D11::D3D11Resources> _d3d11Resources;
	ID3D11DeviceContext* _context;
	ID3D11Device* _d3d11Device;
	IDXGISwapChain* _swapChain;
	ID3D11RenderTargetView* _renderTargetView;
};
}

#define AppEntryPoint(BaseFrameWorkClass) \
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)\
{\
	try\
	{\
		return Windows::App(new BaseFrameWorkClass()).Entry();\
	}\
	catch (const std::exception& ex)\
	{\
		std::cerr << ex.what() << std::endl;\
	}\
	return -1;\
}