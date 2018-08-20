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
		void LoadDefaultImage();
		void GetSourceImageGray();
		void ApplyImageBlur();
		void GetBinaryImage();
		void FindOuterEllipse();
		void DrawEllipseAxis();


		void ShowBestFitEllipse();

		int m_LengthOfBlurSqureSide = 7;// 21;
		int m_ThresholdValue        = 90; // For Binary Image before extracting Outer Ellipse
		int m_CannyThreshValue      = 100; // For Canny Edge Detect before FindContours

		int m_iCannyThreshRatio     = 20;
		double m_dCannyThreshRatio  = 2.0;

		//int m_ImageFlags = cv::WINDOW_AUTOSIZE;
		int m_ImageFlags = cv::WINDOW_NORMAL;
		const char* m_ConstWindowName = "To Extract Ellipse";
		const char* m_OriginalWindowName = "Original";
		
		const std::string m_ImagePath = "../../WatchMe/Images/ExtractEllipse/";
		const std::string m_ImageExtension = ".png";
		//const std::string m_ImageName = "Ellipse_-30";
		//const std::string m_ImagePath = "../WatchMe/Images/";
		//const std::string m_ImageExtension = ".jpg";
		const std::string m_ImageName = "ProjectorAlignment_RetroTarget";

		cv::RotatedRect m_cvEllipse;

		cv::Mat m_Src;
		cv::Mat m_SrcGray;
		cv::Mat m_SrcRGB;
		cv::Mat m_Blurred;
		cv::Mat m_Dst;
		cv::Mat m_DstRGB;
		cv::Mat m_Binary;
		cv::Mat m_CannyOutput;

		std::vector<std::vector<cv::Point> > m_Contours;
		std::vector<cv::Vec4i> m_Hierarchy;

		int m_EllipseContoursIndex = -1;
	};

}



#endif // __SLBINARYELLIPSECORRELATION__
