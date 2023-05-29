#include "DXException.h"
#include "dxerr.h"
#include <sstream>

using namespace Hardware::DX;

//-----------------------------------------------------------------------------
// Exceptions
//-----------------------------------------------------------------------------
InfoException::InfoException(
    int line, const char* file,
    const std::vector<std::string>& infoMsgs) noexcept
    : Exception(line, file)
{
    for (const auto& msg : infoMsgs) {
        m_info += msg;
        m_info.push_back('\n');
    }
    if (!infoMsgs.empty()) {
        m_info.pop_back();
    }
}
const char* InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << "\n[ErrorInfo] " << GetErrorInfo() << "\n\n";
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}
const char* InfoException::GetType() const noexcept
{
    return "DX::InfoException";
}
std::string InfoException::GetErrorInfo() const noexcept
{
    return m_info;
}
//-----------------------------------------------------------------------------
HrException::HrException(int line, const char* file, HRESULT hr,
                                   std::vector<std::string> infoMsgs) noexcept
    : Exception(line, file),
    m_hr(hr)
{
    for (const auto& msg : infoMsgs) {
        m_info += msg;
        m_info.push_back('\n');
    }
    if (!infoMsgs.empty()) {
        m_info.pop_back();
    }
}
const char* HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << "\n[ErrorCode] 0x" << std::hex << std::uppercase
        << GetErrorCode() << std::dec << " (" << (unsigned long) GetErrorCode()
        << ")\n"
        << "[ErrorString] " << GetErrorString() << '\n'
        << "[Description] " << GetErrorDescription() << '\n';
    if (!m_info.empty()) {
        oss << "[ErrorInfo] " << GetErrorInfo() << "\n\n";
    }
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}
const char* HrException::GetType() const noexcept
{
    return "DX::HrException";
}
HRESULT HrException::GetErrorCode() const noexcept { return m_hr; }
std::string HrException::GetErrorString() const noexcept
{
    return DXGetErrorStringA(m_hr);
}
std::string HrException::GetErrorDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescriptionA(m_hr, buf, sizeof(buf));
    return buf;
}
std::string HrException::GetErrorInfo() const noexcept
{
    return m_info;
}
//-----------------------------------------------------------------------------
const char* DeviceRemovedException::GetType() const noexcept
{
    return "DX::DeviceRemovedException";
}
//-----------------------------------------------------------------------------