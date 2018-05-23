#include "stdafx.h"
#include "SLBinaryEllipseCorrelation.h"
#include <iostream>

slopencv::SLBinaryEllipseCorrelation* ptrThreshInstance;

namespace slopencv
{
	extern "C" void ptrThreshold_Demo(int a, void* b)
	{
		::ptrThreshInstance->Threshold_Demo(a, b);
	}

	void SLBinaryEllipseCorrelation::ShowThreshold()
	{
		::ptrThreshInstance = this;

		cv::namedWindow(m_ConstWindowName, m_ImageFlags); // Create a window to display results
		cv::resizeWindow(m_ConstWindowName, 600, 680);

		cv::createTrackbar(trackbar_type,
			m_ConstWindowName, &sideBlock,
			70, ptrThreshold_Demo); // Create Trackbar to choose type of Threshold

		cv::createTrackbar(trackbar_value,
			m_ConstWindowName, &threshold_value,
			max_value, ptrThreshold_Demo); // Create Trackbar to choose Threshold value
										   //! [trackbar]

		Threshold_Demo(0, 0); // Call the function to initialize

							  /// Wait until user finishes program
		for (;;)
		{
			char c = (char)cv::waitKey(20);
			if (c == 27)
			{
				break;
			}
		}
	}

	void SLBinaryEllipseCorrelation::Threshold_Demo(int, void*)
	{
		/* 0: Binary
		1: Binary Inverted
		2: Threshold Truncated
		3: Threshold to Zero
		4: Threshold to Zero Inverted
		*/

		cv::blur(m_Src, m_Blurred, cv::Size(sideBlock, sideBlock), cv::Point(-1, -1)); // blur sideBlock = 15, 13

		//sideBlock = (sideBlock % 2) == 0 ? (sideBlock + 1) : sideBlock; // GaussianBlur sideBlock = 35
		//cv::GaussianBlur(m_Src, m_Blurred, cv::Size(sideBlock, sideBlock), 0, 0);

		cv::threshold(m_Blurred, m_Dst, threshold_value, max_BINARY_value, threshold_type);
		cv::imshow(m_ConstWindowName, m_Dst);
	}

	int SLBinaryEllipseCorrelation::display_caption(const char* caption)
	{
		m_Dst = cv::Mat::zeros(m_Src.size(), m_Src.type());
		cv::putText(m_Dst, caption,
			cv::Point(m_Src.cols / 4, m_Src.rows / 2),
			cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255));

		return display_dst("", DELAY_CAPTION);
	}

	int SLBinaryEllipseCorrelation::display_dst(const char* windowName, int delay)
	{
		cv::imshow(windowName, m_Dst);
		int c = cv::waitKey(delay);
		if (c >= 0) { return -1; }
		return 0;
	}

	void SLBinaryEllipseCorrelation::ShowGaussianAdaptiveThresholding()
	{
		/// Applying Gaussian blur
		for (int i = 15; i < MAX_KERNEL_LENGTH; i = i + 2)
		{
			cv::GaussianBlur(m_Src, m_Dst, cv::Size(i, i), 0, 0);
			display_dst("Gaussian Blur", DELAY_BLUR);
			cv::moveWindow("Gaussian Blur", m_Dst.cols + 350, 170);

			m_Dst = m_Src - m_Dst;
			display_dst("Adaptive Thresholding", DELAY_BLUR);
			cv::moveWindow("Adaptive Thresholding", m_Dst.cols + 350, m_Dst.rows + 200);
		}
	}


	void SLBinaryEllipseCorrelation::ShowWidget()
	{
		m_Src = cv::imread(m_FileName, cv::IMREAD_GRAYSCALE);
		assert(!m_Src.empty());

		cv::equalizeHist(m_Src, m_Src);
		m_Dst = m_Src.clone();

		cv::namedWindow("Original", m_ImageFlags);
		cv::imshow("Original", m_Src);
		cv::resizeWindow("Original", 600, 600);
		cv::moveWindow("Original", 300, 270);

		m_Src = m_Dst.clone();


		ShowThreshold();
		//ShowGaussianAdaptiveThresholding();
		
		cv::waitKey();
	}




} // End of namespace slopencv
