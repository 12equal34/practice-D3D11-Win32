#include "DxgiInfoManager.h"
#include "Window.h"
#include "Renderer.h"

#pragma comment(lib, "dxguid.lib")

using namespace Hardware;
using namespace Hardware::DX;

DxgiInfoManager::DxgiInfoManager()
{
	// define function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// load the dll that contains the function DXGIGetDebugInterface
	const auto hModDxgiDebug = LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
	{
		throw Window::HrException(__LINE__, __FILE__, GetLastError());
	}

	// get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	if (DxgiGetDebugInterface == nullptr)
	{
		throw Window::HrException(__LINE__, __FILE__, GetLastError());
	}

	HRESULT hr = DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), (void**)&m_pDxgiInfoQueue);
	if (FAILED(hr))
	{
		throw Renderer::HrException(__LINE__, __FILE__, hr);
	}
}

void DxgiInfoManager::Set() noexcept
{
	// set the index (next) so that the next all to GetMessages()
	// will only get errors generated after this call
	m_next = m_pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = m_pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = m_next; i < end; i++)
	{
		SIZE_T messageLength {};
		// get the size of message i in bytes
		HRESULT hr = m_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength);
		if (FAILED(hr)) {
			throw Renderer::HrException(__LINE__, __FILE__, hr);
		}
		// allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// get the message and push its description into the vector
		hr = m_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength);
		if (FAILED(hr)) {
			throw Renderer::HrException(__LINE__, __FILE__, hr);
		}
		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}
