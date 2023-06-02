#pragma once
#include "DXException.h"

#ifndef NDEBUG
#include "DxgiInfoManager.h"

#define ThrowIfFailed(hr) \
    DxgiInfoManager::ThrowIfFailed((hr), __LINE__, __FILE__)
#define ThrowIfInfoGot(call)    \
    DxgiInfoManager::Setting(); \
    (call);                     \
    DxgiInfoManager::ThrowIfInfoGot(__LINE__, __FILE__);

#else
#define ThrowIfFailed(hr)    if (FAILED((hr))) {
if ((hr) == DXGI_ERROR_DEVICE_REMOVED) {
    throw DeviceRemovedException(__LINE__, __FILE__, (hr));
} else {
    throw HrException(__LINE__, __FILE__, (hr));
}
}
#define ThrowIfInfoGot(call) (call)
#endif