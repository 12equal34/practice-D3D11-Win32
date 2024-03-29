#include "DxgiInfoManager.h"
#include "Window.h"
#include "DXException.h"

#pragma comment(lib, "dxguid.lib")

namespace hdx = Hardware::DX;

hdx::DxgiInfoManager hdx::DxgiInfoManager::Instance;

void hdx::DxgiInfoManager::Initialize()
{
    // define function signature of DXGIGetDebugInterface
    typedef HRESULT(WINAPI * DXGIGetDebugInterface)(REFIID, void**);

    // load the dll that contains the function DXGIGetDebugInterface
    const auto hModDxgiDebug =
        LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (hModDxgiDebug == nullptr) {
        throw Window::HrException(__LINE__, __FILE__, GetLastError());
    }

    // get address of DXGIGetDebugInterface in dll
    const auto DxgiGetDebugInterface =
        reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(
            GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface")));
    if (DxgiGetDebugInterface == nullptr) {
        throw Window::HrException(__LINE__, __FILE__, GetLastError());
    }

    HRESULT hr = DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue),
                                       (void**)&Instance.m_pDxgiInfoQueue);
    if (FAILED(hr)) {
        throw HrException(__LINE__, __FILE__, hr);
    }
}

void hdx::DxgiInfoManager::Set() noexcept
{
    // set the index (next) so that the next all to GetMessages()
    // will only get errors generated after this call
    m_next = m_pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> hdx::DxgiInfoManager::GetMessages() const
{
    std::vector<std::string> messages;
    const auto end = m_pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
    for (auto i = m_next; i < end; i++) {
        SIZE_T messageLength {};
        // get the size of message i in bytes
        HRESULT hr = m_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr,
                                                  &messageLength);
        if (FAILED(hr)) {
            throw HrException(__LINE__, __FILE__, hr);
        }
        // allocate memory for message
        auto bytes    = std::make_unique<byte[]>(messageLength);
        auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
        // get the message and push its description into the vector
        hr = m_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage,
                                          &messageLength);
        if (FAILED(hr)) {
            throw HrException(__LINE__, __FILE__, hr);
        }
        messages.emplace_back(pMessage->pDescription);
    }
    return messages;
}

void hdx::DxgiInfoManager::ThrowIfFailed(HRESULT hr, int line, const char* file)
{
    Instance.Set();
    if (FAILED(hr)) {
        if (hr == DXGI_ERROR_DEVICE_REMOVED) {
            throw DeviceRemovedException(line, file, hr,
                                         Instance.GetMessages());
        } else {
            throw HrException(line, file, hr, Instance.GetMessages());
        }
    }
}
void hdx::DxgiInfoManager::ThrowIfInfoGot(int line, const char* file)
{
    auto v = Instance.GetMessages();
    if (!v.empty()) {
        throw InfoException(line, file, v);
    }
}

void hdx::DxgiInfoManager::Setting() noexcept { Instance.Set(); }
