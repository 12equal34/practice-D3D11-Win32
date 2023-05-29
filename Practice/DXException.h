#pragma once
#include "BaseException.h"
#include "WindowsHeader.h"
#include <vector>
#include <string>

namespace Hardware::DX
{
class Exception : public BaseException
{
    using BaseException::BaseException;
};

class InfoException : public Exception
{
public:
    InfoException(int line, const char* file,
                  const std::vector<std::string>& infoMsgs) noexcept;
    const char* what() const noexcept override;
    const char* GetType() const noexcept override;
    std::string GetErrorInfo() const noexcept;
private:
    std::string m_info;
};

class HrException : public Exception
{
public:
    HrException(int line, const char* file, HRESULT hr,
                std::vector<std::string> infoMsgs = {}) noexcept;
    const char* what() const noexcept override;
    const char* GetType() const noexcept override;
    HRESULT     GetErrorCode() const noexcept;
    std::string GetErrorString() const noexcept;
    std::string GetErrorDescription() const noexcept;
    std::string GetErrorInfo() const noexcept;
private:
    HRESULT     m_hr;
    std::string m_info;
};

class DeviceRemovedException : public HrException
{
    using HrException::HrException;
public:
    const char* GetType() const noexcept override;
};
}