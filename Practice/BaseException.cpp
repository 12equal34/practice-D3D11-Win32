#include "BaseException.h"
#include <sstream>

BaseException::BaseException(int line, const char* file) noexcept
    : m_line(line),
      m_file(file)
{
}

const char* BaseException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << '\n'
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* BaseException::GetType() const noexcept { return "BaseException"; }

int BaseException::GetLine() const noexcept { return m_line; }

const std::string& BaseException::GetFile() const noexcept { return m_file; }

std::string BaseException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << m_file << '\n'
        << "[Line] " << m_line;
    return oss.str();
}
