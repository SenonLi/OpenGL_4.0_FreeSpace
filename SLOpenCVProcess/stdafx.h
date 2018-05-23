// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// TODO: reference additional headers your program requires here

#include <tchar.h>
#include <string>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

namespace slopencv
{
	static const int           MAX_CPU_SINGLE_CHANNEL_VALUE_INT          = 255;
	static const unsigned int  MAX_CPU_SINGLE_CHANNEL_VALUE_UNSIGNED_INT = 255;

	static const cv::Scalar CV_COLOR_SCALAR_RED = cv::Scalar(0, 0, 255);

	static const int           MAX_CANNY_THRESH_RATIO_INT                = 35;

}
