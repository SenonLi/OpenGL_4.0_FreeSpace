#ifndef __SLBINARYELLIPSECORRELATION__
#define __SLBINARYELLIPSECORRELATION__
#pragma once

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

namespace slopencv
{
	class SLBinaryEllipseCorrelation
	{
	public:
		int DELAY_CAPTION = 1500; // time msec
		int DELAY_BLUR    = 400;  // time msec
		int MAX_KERNEL_LENGTH = 31;

		int threshold_value = 127;
		int threshold_type = 0;
		int sideBlock = 15;

		int const max_value = 255;
		int const max_type = 4;
		int const max_BINARY_value = 255;

		const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
		const char* trackbar_value = "Value";

		void Threshold_Demo(int, void*);
		void ShowWidget();

	private:
		void ShowThreshold();
		void ShowGaussianAdaptiveThresholding();
		int display_caption(const char* caption);
		int display_dst(const char* windowName, int delay);

		int m_ImageFlags = cv::WINDOW_AUTOSIZE;
		const char* m_FileName = "../WatchMe/Images/poor_1.bmp";
		//const char* m_FileName = "../WatchMe/Images/bad_2.bmp";
		const char* m_ConstWindowName = "Threshold";

		cv::Mat m_Src;
		cv::Mat m_Blurred;
		cv::Mat m_Dst;
	};

}



#endif // __SLBINARYELLIPSECORRELATION__
