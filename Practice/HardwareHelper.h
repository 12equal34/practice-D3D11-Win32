#pragma once

namespace Hardware::Helper
{
template <class T>
static void TrimBuffer(std::queue<T>& buffer,
                       unsigned int   maxBufferSize = 16u) noexcept
{
    while (buffer.size() > maxBufferSize) {
        buffer.pop();
    }
}
}