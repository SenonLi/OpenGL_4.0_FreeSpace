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
		int DELAY_CAPTION = 1500;
		int DELAY_BLUR = 400;
		int MAX_KERNEL_LENGTH = 31;

		void ShowWidget();

	private:
		void ShowGaussianAdaptiveThresholding();
		int display_caption(const char* caption);
		int display_dst(const char* windowName, int delay);

		const char* m_FileName = "../WatchMe/Images/poor_3.bmp";

		cv::Mat m_Src;
		cv::Mat m_Dst;
	};

}



#endif // __SLBINARYELLIPSECORRELATION__
