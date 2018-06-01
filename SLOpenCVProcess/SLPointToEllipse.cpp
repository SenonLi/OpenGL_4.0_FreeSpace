#include "stdafx.h"
#include "SLPointToEllipse.h"
#include <iostream>
slopencv::SLPointToEllipse* ptrSLPointToEllipseInstance;

namespace slopencv
{
	extern "C" void FunPtrPaintEllipseWidgetCallBack(int pos, void* userData)
	{
		::ptrSLPointToEllipseInstance->PaintWidgetCallBack(pos, userData);
	}

	void SLPointToEllipse::PaintScreen()
	{
		cv::Mat showEllipse;
		cv::flip(m_EllipseRGB, showEllipse, 1);
		cv::Point2f rotationCenter(m_EllipseRGB.cols / 2.0f, m_EllipseRGB.rows / 2.0f);
		cv::Mat rot_mat = cv::getRotationMatrix2D(rotationCenter, 180.0f, 1.0);
		cv::warpAffine(showEllipse, showEllipse, rot_mat, m_EllipseRGB.size());
		cv::imshow(m_ConstWindowName, showEllipse);
	}

	void SLPointToEllipse::DrawPoint()
	{
		m_RandomPoint = cv::Point(m_RandomPoint_x, m_RandomPoint_y);
		cv::circle(m_EllipseRGB, m_RandomPoint, 10, slopencv::CV_COLOR_SCALAR_BLUE, CV_FILLED, 8);
	}

	void SLPointToEllipse::DetermineShortestDistanceFromPointToEllipse()
	{
		double phiShortest = CV_PI / 2.0; // starting point has to be 90 degree, if start from 0, iteration may return two same values due to sin(0) == 0
		double shortestDistance = 999999.0;
		double newDistance = shortestDistance - 2.0;

		double x_RelativePoint = -1.0;
		double y_RelativePoint = -1.0;
		slopencv::GetPointRelativeToEllipse(m_RandomPoint_x, m_RandomPoint_y, x_RelativePoint, y_RelativePoint, m_Ellipse_x0, m_Ellipse_y0, Ellipse_Angle / 180.0 * CV_PI);
		int iterationCount = 0;

		while (shortestDistance - newDistance > 1)
		{
			shortestDistance = newDistance;
			phiShortest = slopencv::IteratePhiForShortestDistanceToEllipse(x_RelativePoint, y_RelativePoint, m_Ellipse_a, m_Ellipse_b, phiShortest);
			newDistance = slopencv::GetDistanceFromPointToPoint(abs(x_RelativePoint), abs(y_RelativePoint), m_Ellipse_a * cos(phiShortest), m_Ellipse_b * sin(phiShortest));
			iterationCount++;
		}

		m_Distance = shortestDistance;
		std::cout << "Shortest Distance : \t " << shortestDistance << " , \t Iteratio Count : \t " << iterationCount << "\t Times !!\n";
	}

	void SLPointToEllipse::DrawDistanceCircle()
	{
		cv::circle(m_EllipseRGB, m_RandomPoint, (int)m_Distance, slopencv::CV_COLOR_SCALAR_BLACK, 3, cv::LINE_8);
	}
	void SLPointToEllipse::PaintWidgetCallBack(int /*pos*/, void* /*userData*/)
	{
		DrawBasicEllipse();
		DrawPoint();
		//DetermineShortestDistanceFromPointToEllipse();

		m_Ellipse.center = cv::Size2f((float)m_Ellipse_x0, (float)m_Ellipse_y0);
		m_Distance = slopencv::GetShortestDistanceFromPointToEllipse(cv::Point(m_RandomPoint_x, m_RandomPoint_y), m_Ellipse);

		DrawDistanceCircle();

		PaintScreen();
	}

	void SLPointToEllipse::InitialBasicEllipse()
	{
		m_Ellipse.angle = Ellipse_Angle;
		m_Ellipse.size = cv::Size(m_Ellipse_a * 2, m_Ellipse_b * 2);
	}

	void SLPointToEllipse::ShowWidget()
	{
		::ptrSLPointToEllipseInstance = this;

		cv::namedWindow(m_ConstWindowName, m_ImageFlags); // Create a window to display results
		cv::resizeWindow(m_ConstWindowName, WIDGET_SIZE_WIDTH, WIDGET_SIZE_HEIGHT + 130);

		cv::createTrackbar("x_ellipse", m_ConstWindowName, &m_Ellipse_x0, WIDGET_SIZE_WIDTH, FunPtrPaintEllipseWidgetCallBack);
		cv::createTrackbar("y_ellipse", m_ConstWindowName, &m_Ellipse_y0, WIDGET_SIZE_WIDTH, FunPtrPaintEllipseWidgetCallBack);
		cv::createTrackbar("x_point", m_ConstWindowName, &m_RandomPoint_x, WIDGET_SIZE_WIDTH, FunPtrPaintEllipseWidgetCallBack);
		cv::createTrackbar("y_point", m_ConstWindowName, &m_RandomPoint_y, WIDGET_SIZE_WIDTH, FunPtrPaintEllipseWidgetCallBack);

		InitialBasicEllipse();

		PaintWidgetCallBack();
		cv::waitKey();
	}



	void SLPointToEllipse::DrawBasicEllipse()
	{
		m_EllipseRGB = cv::Mat(800, 800, CV_8UC3, slopencv::CV_COLOR_SCALAR_EyeProtection);
		m_Ellipse.center = cv::Size2f((float)m_Ellipse_x0, (float)m_Ellipse_y0);
		cv::ellipse(m_EllipseRGB, m_Ellipse, slopencv::CV_COLOR_SCALAR_BLUE, 3, 8);
	}






}// End of namespace slopencv

