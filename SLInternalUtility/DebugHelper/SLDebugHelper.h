#ifndef __SLDEBUGHELPER__
#define __SLDEBUGHELPER__
#pragma once

#include <Windows.h>   // For both OutputDebugString() and declaration of type BYTE
#include <sstream>     // For construct Output stringstream

#if defined(_DEBUG) || defined(SL_ReleaseDebug)
#define SLOutputDebugString(message) OutputDebugString(message)
#else
#define SLOutputDebugString(message)
#endif

namespace slutil
{


}






#endif // __SLDEBUGHELPER__