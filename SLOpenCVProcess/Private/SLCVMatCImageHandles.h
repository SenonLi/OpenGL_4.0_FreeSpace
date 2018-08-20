#ifndef __SLCVMATCIMAGEHANDLES__
#define __SLCVMATCIMAGEHANDLES__
#pragma once

#include "SLCImageProcess.h"

namespace slopencv
{
	void ConvertCImageToCVMat(CImage& src, cv::Mat& dst);
	void ConvertCVMatToCImage(const cv::Mat& in, CImage& out);


}; // end of namespace slopencv

#endif // __SLCVMATCIMAGEHANDLES__