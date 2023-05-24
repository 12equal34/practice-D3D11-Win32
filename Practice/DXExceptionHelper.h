#pragma once
#include "WindowsHeader.h"
#include "Renderer.h"

#ifndef NDEBUG
#include "DxgiInfoManager.h"
#endif

namespace Hardware::DX
{
#ifndef NDEBUG
static DxgiInfoManager g_DxgiInfoManager;
#endif

inline void ThrowIfFailed(HRESULT hr, int line, const char* file)
{
#ifndef NDEBUG
    g_DxgiInfoManager.Set();
    if (FAILED(hr)) {
        if (hr == DXGI_ERROR_DEVICE_REMOVED) {
            throw Renderer::DeviceRemovedException(
                line, file, hr, g_DxgiInfoManager.GetMessages());
        } else {
            throw Renderer::HrException(line, file, hr,
                                        g_DxgiInfoManager.GetMessages());
        }
    }
#else
    if (FAILED(hr)) {
        if (hr == DXGI_ERROR_DEVICE_REMOVED) {
            throw Renderer::DeviceRemovedException(line, file, hr);
        } else {
            throw Renderer::HrException(line, file, hr);
        }
    }
#endif
}

inline void ThrowIfInfoGot(int line, const char* file)
{
#ifndef NDEBUG
    auto v = g_DxgiInfoManager.GetMessages();
    if (!v.empty()) {
        throw Renderer::InfoException(line, file, v);
    }
#endif
}
}

#define ThrowIfFailed(hr) ThrowIfFailed((hr), __LINE__, __FILE__)
#ifndef NDEBUG
#define ThrowIfInfoGot(call) \
    g_DxgiInfoManager.Set(); \
    (call);                  \
    ThrowIfInfoGot(__LINE__, __FILE__);
#else
#define ThrowIfInfoGot(call) (call)
#endif