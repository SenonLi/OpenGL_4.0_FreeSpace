#include "stdafx.h"
#include "SLBinaryEllipseCorrelation.h"
#include <iostream>

slopencv::SLBinaryEllipseCorrelation* ptrThreshInstance;

namespace slopencv
{
	extern "C" void FunPtrApplyThreshold(int pos, void* userData)
	{
		::ptrThreshInstance->ApplyImageThreshold(pos, userData);
	}

	/// <summary>Thresholding after different Blur methods, with Blur Area and Threshold Value controled by Trackbar </summary>
	/// <remarks>Blur Methods: HomogeneousBlur (average), GaussianBlur</remarks>
	void SLBinaryEllipseCorrelation::ShowThresholdingAfterBlur()
	{
		::ptrThreshInstance = this;
		cv::namedWindow(m_ConstWindowName, m_ImageFlags); // Create a window to display results
		//cv::resizeWindow(m_ConstWindowName, 600, 680);
		// Create Trackbar to choose m_LengthOfBlurSqureSide of Image Blure
		cv::createTrackbar("BlurSize", m_ConstWindowName, &m_LengthOfBlurSqureSide, 
			MAX_LENGTH_OF_BLUR_SQUARE_SIDE, FunPtrApplyThreshold);
		// Create Trackbar to choose Threshold value
		cv::createTrackbar("Thresh", m_ConstWindowName, &m_ThresholdValue, 
			slopencv::MAX_CPU_SINGLE_CHANNEL_VALUE_INT, FunPtrApplyThreshold);

		ApplyImageBlur();
		ApplyImageThreshold();
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

	void SLBinaryEllipseCorrelation::ApplyImageThreshold(int /*pos*/, void* /*userData*/)
	{
		cv::threshold(m_Blurred, m_Dst, m_ThresholdValue, slopencv::MAX_CPU_SINGLE_CHANNEL_VALUE_INT, cv::THRESH_BINARY);
		cv::imshow(m_ConstWindowName, m_Dst);
		cv::moveWindow(m_ConstWindowName, 550, 190);
	}

	void SLBinaryEllipseCorrelation::ShowWidget()
	{
		m_Src = cv::imread(m_FileName, cv::IMREAD_GRAYSCALE);
		assert(!m_Src.empty());

		// Histogram-Equalize Image and show it
		cv::equalizeHist(m_Src, m_Src);
		m_Dst = m_Src.clone();
		cv::namedWindow("Original", m_ImageFlags);
		cv::imshow("Original", m_Src);
		//cv::resizeWindow("Original", 600, 600);
		cv::moveWindow("Original", 300, 270);


		// Begin of OpenCV Process
		ShowThresholdingAfterBlur();
		
		cv::waitKey();
	}




} // End of namespace slopencv
