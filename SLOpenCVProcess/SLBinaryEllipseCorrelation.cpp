#include "stdafx.h"
#include "SLBinaryEllipseCorrelation.h"
#include <iostream>

#include <codecvt>

slopencv::SLBinaryEllipseCorrelation* ptrBinaryEllipseCorrelationInstance;

namespace slopencv
{
	extern "C" void FunPtrPaintWidgetCallBack(int pos, void* userData)
	{
		::ptrBinaryEllipseCorrelationInstance->PaintWidgetCallBack(pos, userData);
	}

	std::string ws2s(const std::wstring& wstr)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}
	std::wstring s2ws(const std::string& str)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	void SLBinaryEllipseCorrelation::LoadDefaultImage()
	{
		// Get basic image info
		CImage imageLoader;
		imageLoader.Load((s2ws(m_ImagePath + m_ImageName + m_ImageExtension)).c_str());
		assert(!imageLoader.IsNull());

		slopencv::GetImageMat(imageLoader, m_Src);
		cv::imshow(m_OriginalWindowName, m_Src);




		//cv::Mat testCV_64FC1(m_Src.size(), CV_64FC1);
		//for (int row = 0; row < m_Src.rows; row++)
		//{
		//	for (int col = 0; col < m_Src.cols; col++)
		//	{
		//		testCV_64FC1.at<double>(cv::Point(col, row)) = m_Src.at<cv::Vec4b>(cv::Point(col, row))[0] / 255.0;
		//	}
		//}

		//cv::Mat blurredCV_64FC1;
		//cv::GaussianBlur(m_SrcGray, blurredCV_64FC1, cv::Size(m_LengthOfBlurSqureSide, m_LengthOfBlurSqureSide), 0, 0);

		//cv::namedWindow("m_CV_32FC1_RGB", m_ImageFlags);

		//cv::imshow("m_CV_32FC1_RGB", testCV_64FC1);


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
		//cv::equalizeHist(m_SrcGray, m_SrcGray);
		m_Dst = m_SrcGray.clone();
		cvtColor(m_SrcGray, m_SrcRGB, cv::COLOR_GRAY2RGB);

		cv::namedWindow(m_OriginalWindowName, m_ImageFlags);
		// Create Trackbar to choose m_LengthOfBlurSqureSide of Image Blure
		cv::createTrackbar("Thresh", m_OriginalWindowName, &m_CannyThreshValue,
			slopencv::MAX_CPU_SINGLE_CHANNEL_VALUE_INT, FunPtrPaintWidgetCallBack);
		// Create Trackbar to choose Threshold value
		cv::createTrackbar("Ratio", m_OriginalWindowName, &m_iCannyThreshRatio,
			slopencv::MAX_CANNY_THRESH_RATIO_INT, FunPtrPaintWidgetCallBack);

		cv::resizeWindow(m_OriginalWindowName, 600, 600);
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
	
		//m_Blurred = m_SrcGray + cv::Mat(m_SrcGray.size(), m_SrcGray.type(), cv::Scalar(127)) - m_Blurred;

		//cv::namedWindow("Blured", m_ImageFlags); // Create a window to display results
		//cv::resizeWindow("Blured", 600, 680);
		//cv::imshow("Blured", m_Blurred);

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

	void SLBinaryEllipseCorrelation::DrawEllipseAxis()
	{

		double majorAxisLength = m_cvEllipse.size.width / 2.0;
		double minorAxisLength = m_cvEllipse.size.height / 2.0;

		//cv::rectangle(m_DstRGB, m_cvEllipse.boundingRect2f(), slopencv::CV_COLOR_SCALAR_YELLOW, 3, 8);
		cv::Point2f center = m_cvEllipse.center;
		cv::Point2f vectorMajorAxis(1.0, tanf((m_cvEllipse.angle) / 180.0f * (float)CV_PI));
		cv::Point2f vectorMinorAxis(1.0, tanf((m_cvEllipse.angle - 90.0f) / 180.0f * (float)CV_PI));
		float majorDistance = sqrt(vectorMajorAxis.x * vectorMajorAxis.x + vectorMajorAxis.y * vectorMajorAxis.y);
		float minorDistance = sqrt(vectorMinorAxis.x * vectorMinorAxis.x + vectorMinorAxis.y * vectorMinorAxis.y);
		cv::Point2f normalMajorAxis = vectorMajorAxis / majorDistance;
		cv::Point2f normalMinorAxis = vectorMinorAxis / minorDistance;
		cv::Point2f majorPoint = center + normalMajorAxis * majorAxisLength;
		cv::Point2f minorPoint = center + normalMinorAxis * minorAxisLength;

		cv::line(m_DstRGB, center, majorPoint, slopencv::CV_COLOR_SCALAR_BLUE, 3, 8);
		cv::line(m_DstRGB, center, minorPoint, slopencv::CV_COLOR_SCALAR_BLUE, 3, 8);


		//cv::imshow(m_ConstWindowName, m_DstRGB);

	}

	void SLBinaryEllipseCorrelation::FindOuterEllipse()
	{
		/// Detect edges using canny
		m_dCannyThreshRatio = m_iCannyThreshRatio / 10.0;
		cv::Canny(m_Binary, m_CannyOutput, m_CannyThreshValue, m_CannyThreshValue * m_dCannyThreshRatio, 3);
		//cv::imshow(m_ConstWindowName, m_CannyOutput);


		/// Find contours
		cv::findContours(m_CannyOutput, m_Contours, m_Hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
		/// Draw contours
		cvtColor(m_SrcGray, m_SrcRGB, cv::COLOR_GRAY2RGB);
		int levelZeroCount = 0;
		int levelOneCount = 0;
		int levelTwoCount = 0;
		int otherLevelCount = 0;
		size_t maxEllipseContourSize = 0;
		int maxEllipseContourIndex = m_EllipseContoursIndex;
		for (int i = 0; i< m_Contours.size(); i++)
		{
			if (m_Hierarchy[i][3] == 0)
			{
				cv::drawContours(m_SrcRGB, m_Contours, i, slopencv::CV_COLOR_SCALAR_BLUE, 2, 8, m_Hierarchy, 0);
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
			else if (m_Hierarchy[i][3] == -1)
			{
				cv::drawContours(m_SrcRGB, m_Contours, i, slopencv::CV_COLOR_SCALAR_RED, 1, 8, m_Hierarchy, 0);
				otherLevelCount++;
			}
			else
			{
				cv::drawContours(m_SrcRGB, m_Contours, i, slopencv::CV_COLOR_SCALAR_WHITE, 1, 8, m_Hierarchy, 0);
				otherLevelCount++;
			}

			if (m_Contours[i].size() > maxEllipseContourSize)
			{
				maxEllipseContourSize = m_Contours[i].size();
				maxEllipseContourIndex = i;
			}
		}
		m_EllipseContoursIndex = maxEllipseContourIndex;
		cv::imshow(m_OriginalWindowName, m_SrcRGB);
		cv::namedWindow("Outer Contours", m_ImageFlags); // Create a window to display results
		cv::resizeWindow("Outer Contours", 600, 680);
		cv::imshow("Outer Contours", m_SrcRGB);


		std::cout << "Total Level 0  Contours: \t " << levelZeroCount
			<< "\t Level 1 : \t " << levelOneCount << "\t Level 2 : \t" << levelTwoCount << std::endl;

		if (maxEllipseContourSize > 5)
		{
			m_cvEllipse = cv::fitEllipse(cv::Mat(m_Contours[m_EllipseContoursIndex]));

			cvtColor(m_Dst, m_DstRGB, cv::COLOR_GRAY2RGB);

			//cv::putText(m_DstRGB, "Best-Fit",
			//	cv::Point2f(m_cvEllipse.center.x - 35, m_cvEllipse.center.y - 20),
			//	cv::FONT_HERSHEY_TRIPLEX, 0.6, cv::Scalar(255, 0, 0));
			//cv::putText(m_DstRGB, "Ellipse",
			//	cv::Point2f(m_cvEllipse.center.x - 35, m_cvEllipse.center.y - 00),
			//	cv::FONT_HERSHEY_TRIPLEX, 0.6, cv::Scalar(255, 0, 0));

			cv::ellipse(m_DstRGB, m_cvEllipse, slopencv::CV_COLOR_SCALAR_RED, 2, 8);

			DrawEllipseAxis();


			cv::imshow(m_ConstWindowName, m_DstRGB);

			cv::imwrite(m_ImagePath + m_ImageName + std::string("_cvEllipse") + m_ImageExtension, m_DstRGB);
		}

	}


} // End of namespace slopencv
