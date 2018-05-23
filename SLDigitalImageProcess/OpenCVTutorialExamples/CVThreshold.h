#ifndef __CVTHRESHOLD__
#define __CVTHRESHOLD__
#pragma once

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

class CVThreshold
{
public:
	/// Global variables
	int threshold_value = 0;
	int threshold_type = 3;
	int const max_value = 255;
	int const max_type = 4;
	int const max_BINARY_value = 255;

	const char* window_name = "Threshold Demo";
	const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
	const char* trackbar_value = "Value";

	int showWidget();
	void Threshold_Demo(int, void*);

private:

	cv::Mat m_Src;
	cv::Mat src_gray;
	cv::Mat m_Dst;
};


#endif // __CVTHRESHOLD__
