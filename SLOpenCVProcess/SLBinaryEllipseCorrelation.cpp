#include "stdafx.h"
#include "SLBinaryEllipseCorrelation.h"
#include <iostream>

slopencv::SLBinaryEllipseCorrelation* ptrBinaryEllipseCorrelationInstance;

namespace slopencv
{
	extern "C" void FunPtrPaintWidgetCallBack(int pos, void* userData)
	{
		::ptrBinaryEllipseCorrelationInstance->PaintWidgetCallBack(pos, userData);
	}

	/// <summary>Thresholding after different Blur methods, with Blur Area and Threshold Value controled by Trackbar </summary>
	/// <remarks>Blur Methods: HomogeneousBlur (average), GaussianBlur</remarks>
	void SLBinaryEllipseCorrelation::ShowThresholdingAfterBlur()
	{
		::ptrBinaryEllipseCorrelationInstance = this;
		cv::namedWindow(m_ConstWindowName, m_ImageFlags); // Create a window to display results
		cv::resizeWindow(m_ConstWindowName, 600, 680);
		// Create Trackbar to choose m_LengthOfBlurSqureSide of Image Blure
		cv::createTrackbar("BlurSize", m_ConstWindowName, &m_LengthOfBlurSqureSide, 
			MAX_LENGTH_OF_BLUR_SQUARE_SIDE, FunPtrPaintWidgetCallBack);
		// Create Trackbar to choose Threshold value
		cv::createTrackbar("Thresh", m_ConstWindowName, &m_ThresholdValue, 
			slopencv::MAX_CPU_SINGLE_CHANNEL_VALUE_INT, FunPtrPaintWidgetCallBack);
		cv::moveWindow(m_ConstWindowName, 550, 190);

		// GetBinaryImage, and FindOuterEllipse
		PaintWidgetCallBack();
	}

	/// <summary>Blur Methods: HomogeneousBlur (average), GaussianBlur</summary>
	void SLBinaryEllipseCorrelation::ApplyImageBlur()
	{
		// Best HomogeneousBlur (average) m_LengthOfBlurSqureSide = 15, 13
		cv::blur(m_Src, m_Blurred, cv::Size(m_LengthOfBlurSqureSide, m_LengthOfBlurSqureSide), cv::Point(-1, -1));

		// Best GaussianBlur m_LengthOfBlurSqureSide = 35
		//m_LengthOfBlurSqureSide = (m_LengthOfBlurSqureSide % 2) == 0 ? (m_LengthOfBlurSqureSide + 1) : m_LengthOfBlurSqureSide;
		//cv::GaussianBlur(m_Src, m_Blurred, cv::Size(m_LengthOfBlurSqureSide, m_LengthOfBlurSqureSide), 0, 0);
	}
	void SLBinaryEllipseCorrelation::GetBinaryImage()
	{
		ApplyImageBlur();
		cv::threshold(m_Blurred, m_Dst, m_ThresholdValue, slopencv::MAX_CPU_SINGLE_CHANNEL_VALUE_INT, cv::THRESH_BINARY);
		cv::imshow(m_ConstWindowName, m_Dst);
		m_Binary = m_Dst.clone();
	}

	void SLBinaryEllipseCorrelation::PaintWidgetCallBack(int /*pos*/, void* /*userData*/)
	{
		GetBinaryImage();
		FindOuterEllipse();
	}

	void SLBinaryEllipseCorrelation::FindOuterEllipse()
	{
		/// Detect edges using canny
		m_dCannyThreshRatio = m_iCannyThreshRatio / 10.0;
		cv::Canny(m_Binary, m_CannyOutput, m_CannyThreshValue, m_CannyThreshValue * m_dCannyThreshRatio, 3);
		/// Find contours
		cv::findContours(m_CannyOutput, m_Contours, m_Hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

		/// Draw contours
		cvtColor(m_Src, m_SrcRGB, cv::COLOR_GRAY2RGB);
		for (size_t i = 0; i< m_Contours.size(); i++)
		{
			cv::drawContours(m_SrcRGB, m_Contours, (int)i, slopencv::CV_COLOR_SCALAR_RED, 1, 8, m_Hierarchy, 0);
		}

		cv::imshow(m_OriginalWindowName, m_SrcRGB);
	}

	void SLBinaryEllipseCorrelation::ShowWidget()
	{
		m_Src = cv::imread(m_FileName, cv::IMREAD_GRAYSCALE);
		assert(!m_Src.empty());

		// Histogram-Equalize Image and show it
		cv::equalizeHist(m_Src, m_Src);
		m_Dst = m_Src.clone();
		cvtColor(m_Src, m_SrcRGB, cv::COLOR_GRAY2RGB);

		cv::namedWindow(m_OriginalWindowName, m_ImageFlags);
		// Create Trackbar to choose m_LengthOfBlurSqureSide of Image Blure
		cv::createTrackbar("Thresh", m_OriginalWindowName, &m_CannyThreshValue,
			slopencv::MAX_CPU_SINGLE_CHANNEL_VALUE_INT, FunPtrPaintWidgetCallBack);
		// Create Trackbar to choose Threshold value
		cv::createTrackbar("Ratio", m_OriginalWindowName, &m_iCannyThreshRatio,
			slopencv::MAX_CANNY_THRESH_RATIO_INT, FunPtrPaintWidgetCallBack);

		cv::resizeWindow("Original", 600, 600);
		cv::moveWindow(m_OriginalWindowName, 300, 270);
		cv::imshow(m_OriginalWindowName, m_SrcRGB);


		// Begin of OpenCV Process
		ShowThresholdingAfterBlur();
		
		cv::waitKey();
	}




} // End of namespace slopencv
