#pragma once
#include "WindowsHeader.h"
#include <wrl.h>
#include <dxgidebug.h>
#include <vector>
#include <string>

namespace Hardware::DX
{
class DxgiInfoManager
{
public:
    static void ThrowIfFailed(HRESULT hr, int line, const char* file);
    static void ThrowIfInfoGot(int line, const char* file);
    static void Setting() noexcept;

private:
    DxgiInfoManager();
    ~DxgiInfoManager()                                 = default;
    DxgiInfoManager(const DxgiInfoManager&)            = delete;
    DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;

    void                     Set() noexcept;
    std::vector<std::string> GetMessages() const;

private:
    unsigned long long                     m_next = 0u;
    Microsoft::WRL::ComPtr<IDXGIInfoQueue> m_pDxgiInfoQueue;

    static DxgiInfoManager s_Instance;
};
}
