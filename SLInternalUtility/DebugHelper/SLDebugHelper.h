#ifndef __SLDEBUGHELPER__
#define __SLDEBUGHELPER__
#pragma once

#include <sstream>
#include <Windows.h>

#if defined(_DEBUG) || defined(SL_ReleaseDebug)
#define SLOutputDebugString(message) OutputDebugString(message)
#else
#define SLOutputDebugString(message)
#endif

namespace slutil
{


}






#endif // __SLDEBUGHELPER__