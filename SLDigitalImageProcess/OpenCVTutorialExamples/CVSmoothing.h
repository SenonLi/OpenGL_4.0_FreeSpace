#ifndef __CVSMOOTHING__
#define __CVSMOOTHING__
#pragma once

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

class CVSmoothing
{
public:
	CVSmoothing() {
		window_name = "Smoothing Demo";
	};

	int DELAY_CAPTION = 1500;
	int DELAY_BLUR = 100;
	int MAX_KERNEL_LENGTH = 31;

	int showWidget();

private:
	int display_caption(const char* caption);
	int display_dst(int delay);

	char* window_name;
	cv::Mat src;
	cv::Mat dst;

};


#endif // __CVSMOOTHING__
