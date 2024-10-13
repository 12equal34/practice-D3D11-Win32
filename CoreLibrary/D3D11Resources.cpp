#include "pch.h"
#include "D3D11Resources.h"

#include <exception>

#define ThrowIfFailed(x) if ( FAILED((x)) ) { throw new std::exception{}; }
