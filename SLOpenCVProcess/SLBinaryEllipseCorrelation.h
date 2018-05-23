#ifndef __SLBINARYELLIPSECORRELATION__
#define __SLBINARYELLIPSECORRELATION__
#pragma once

#include "opencv2/highgui.hpp" // For Instancing cv::Mat

namespace slopencv
{
	class SLBinaryEllipseCorrelation
	{
	public:
		const int DELAY_SHOW_BLURRED_IMAGE       = 400;	// time in msec
		const int MAX_LENGTH_OF_BLUR_SQUARE_SIDE = 70;  // in pixel
		
		void PaintWidgetCallBack(int pos = 0, void* userData = nullptr); // Signature here for Trackerbar registration
		void ShowWidget();

	private:
		void ApplyImageBlur();
		void GetBinaryImage();
		void ShowThresholdingAfterBlur();
		void FindOuterEllipse();

		int m_LengthOfBlurSqureSide = 15;
		int m_ThresholdValue        = 127; // For Binary Image before extracting Outer Ellipse
		int m_CannyThreshValue      = 100; // For Canny Edge Detect before FindContours

		int m_iCannyThreshRatio     = 20;
		double m_dCannyThreshRatio  = 2.0;

		//int m_ImageFlags = cv::WINDOW_AUTOSIZE;
		int m_ImageFlags = cv::WINDOW_NORMAL;
		const char* m_ConstWindowName = "Threshold after Blur";
		const char* m_OriginalWindowName = "Original";
		const char* m_FileName = "../WatchMe/Images/poor_3.bmp";
		//const char* m_FileName = "../WatchMe/Images/bad_2.bmp";

		cv::Mat m_Src;
		cv::Mat m_SrcRGB;
		cv::Mat m_Blurred;
		cv::Mat m_Dst;
		cv::Mat m_Binary;
		cv::Mat m_CannyOutput;

		std::vector<std::vector<cv::Point> > m_Contours;
		std::vector<cv::Vec4i> m_Hierarchy;
	};

}



#endif // __SLBINARYELLIPSECORRELATION__
