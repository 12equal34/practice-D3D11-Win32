#pragma once
#include "Window.h"

namespace Hardware::Win
{
template <typename T>
inline void ThrowIfNull(T returnValue, int line, const char* file)
{
    if (returnValue == NULL) {
        throw Window::HrException(line, file, GetLastError());
    }
}
}

#define ThrowIfNull(returnValue) Hardware::Win::ThrowIfNull((returnValue), __LINE__, __FILE__)