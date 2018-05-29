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

	void SLBinaryEllipseCorrelation::LoadDefaultImage()
	{
		//m_Src = cv::imread(m_ImagePath + m_ImageName + m_ImageExtension, cv::IMREAD_GRAYSCALE);
		//assert(!m_Src.empty());

		// Get basic image info
		CImage imageLoader;
		imageLoader.Load(_T("../WatchMe/Images/scan.png"));

		slopencv::GetImageMat(imageLoader, m_Src);

		assert(!m_Src.empty());
	}

	void SLBinaryEllipseCorrelation::GetSourceImageGray()
	{
		assert(!m_Src.empty());
		if(!m_SrcGray.empty())
			m_SrcGray.release();

		switch (m_Src.type())
		{
		case CV_8UC1:
			m_SrcGray = m_Src.clone();
			break;
		case CV_8UC3:
			cvtColor(m_Src, m_SrcGray, cv::COLOR_BGR2GRAY);
			break;
		case CV_8UC4:
			cvtColor(m_Src, m_SrcGray, cv::COLOR_BGRA2GRAY);
			break;
		}
	}

	void SLBinaryEllipseCorrelation::ShowWidget()
	{
		LoadDefaultImage();
		GetSourceImageGray();

		// Histogram-Equalize Image and show it
		cv::equalizeHist(m_SrcGray, m_SrcGray);
		m_Dst = m_SrcGray.clone();
		cvtColor(m_SrcGray, m_SrcRGB, cv::COLOR_GRAY2RGB);

		cv::namedWindow(m_OriginalWindowName, m_ImageFlags);
		// Create Trackbar to choose m_LengthOfBlurSqureSide of Image Blure
		cv::createTrackbar("Thresh", m_OriginalWindowName, &m_CannyThreshValue,
			slopencv::MAX_CPU_SINGLE_CHANNEL_VALUE_INT, FunPtrPaintWidgetCallBack);
		// Create Trackbar to choose Threshold value
		cv::createTrackbar("Ratio", m_OriginalWindowName, &m_iCannyThreshRatio,
			slopencv::MAX_CANNY_THRESH_RATIO_INT, FunPtrPaintWidgetCallBack);

		cv::resizeWindow("Original", 600, 600);
		cv::imshow(m_OriginalWindowName, m_SrcRGB);
		cv::moveWindow(m_OriginalWindowName, 300, 270);


		// Begin of OpenCV Process
		ShowBestFitEllipse();

		cv::waitKey();
	}

	/// <summary>Thresholding after different Blur methods, with Blur Area and Threshold Value controled by Trackbar </summary>
	/// <remarks>Blur Methods: HomogeneousBlur (average), GaussianBlur</remarks>
	void SLBinaryEllipseCorrelation::ShowBestFitEllipse()
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
		// Best HomogeneousBlur (average) m_LengthOfBlurSqureSide = 15, 13, not good for findContours
		//cv::blur(m_SrcGray, m_Blurred, cv::Size(m_LengthOfBlurSqureSide, m_LengthOfBlurSqureSide), cv::Point(-1, -1));

		// Best GaussianBlur m_LengthOfBlurSqureSide = 35
		m_LengthOfBlurSqureSide = (m_LengthOfBlurSqureSide % 2) == 0 ? (m_LengthOfBlurSqureSide + 1) : m_LengthOfBlurSqureSide;
		cv::GaussianBlur(m_SrcGray, m_Blurred, cv::Size(m_LengthOfBlurSqureSide, m_LengthOfBlurSqureSide), 0, 0);
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
		cv::imshow(m_ConstWindowName, m_CannyOutput);

		/// Find contours
		cv::findContours(m_CannyOutput, m_Contours, m_Hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
		/// Draw contours
		cvtColor(m_SrcGray, m_SrcRGB, cv::COLOR_GRAY2RGB);
		int levelZeroCount = 0;
		int levelOneCount = 0;
		int levelTwoCount = 0;
		size_t maxEllipseContourSize = 0;
		int maxEllipseContourIndex = m_EllipseContoursIndex;
		for (int i = 0; i< m_Contours.size(); i++)
		{
			if (m_Hierarchy[i][3] == 0)
			{
				cv::drawContours(m_SrcRGB, m_Contours, i, slopencv::CV_COLOR_SCALAR_BLUE, 2, 8, m_Hierarchy, 0);
				if (m_Contours[i].size() > maxEllipseContourSize)
				{
					maxEllipseContourSize = m_Contours[i].size();
					maxEllipseContourIndex = i;
				}
				levelZeroCount++;
			}
			else if (m_Hierarchy[i][3] == 1)
			{
				cv::drawContours(m_SrcRGB, m_Contours, i, slopencv::CV_COLOR_SCALAR_YELLOW, 2, 8, m_Hierarchy, 0);
				levelOneCount++;
			}
			else if (m_Hierarchy[i][3] == 2)
			{
				cv::drawContours(m_SrcRGB, m_Contours, i, slopencv::CV_COLOR_SCALAR_CYAN, 2, 8, m_Hierarchy, 0);
				levelTwoCount++;
			}
		}
		m_EllipseContoursIndex = maxEllipseContourIndex;
		cv::imshow(m_OriginalWindowName, m_SrcRGB);

		std::cout << "Total Level 0  Contours: \t " << levelZeroCount
			<< "\t Level 1 : \t " << levelOneCount << "\t Level 2 : \t" << levelTwoCount << std::endl;

		if (maxEllipseContourSize > 5)
		{
			cv::RotatedRect ellipseRotatedRect;
			ellipseRotatedRect = cv::fitEllipse(cv::Mat(m_Contours[m_EllipseContoursIndex]));

			cvtColor(m_Dst, m_DstRGB, cv::COLOR_GRAY2RGB);

			cv::putText(m_DstRGB, "Best-Fit",
				cv::Point2f(ellipseRotatedRect.center.x - 35, ellipseRotatedRect.center.y - 20),
				cv::FONT_HERSHEY_TRIPLEX, 0.6, cv::Scalar(255, 0, 0));
			cv::putText(m_DstRGB, "Ellipse",
				cv::Point2f(ellipseRotatedRect.center.x - 35, ellipseRotatedRect.center.y - 00),
				cv::FONT_HERSHEY_TRIPLEX, 0.6, cv::Scalar(255, 0, 0));

			cv::ellipse(m_DstRGB, ellipseRotatedRect, slopencv::CV_COLOR_SCALAR_RED, 2, 8);
			cv::imshow(m_ConstWindowName, m_DstRGB);

			cv::imwrite(m_ImagePath + m_ImageName + std::string("_cvEllipse") + m_ImageExtension, m_DstRGB);
		}

	}


} // End of namespace slopencv
