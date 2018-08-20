#include "../stdafx.h"
#include "../Public/SLPointToEllipse.h"
#include <iostream>

#include "SLOpenCVProcess.h"
#include "StaticConstBasics\SLGeometryBasics.h"

slopencv::SLPointToEllipse* ptrSLPointToEllipseInstance;

namespace slopencv
{
	extern "C" void FunPtrPaintEllipseWidgetCallBack(int pos, void* userData)
	{
		::ptrSLPointToEllipseInstance->PaintWidgetCallBack(pos, userData);
	}

	void SLPointToEllipse::PaintScreen()
	{
		cv::Mat showEllipse = m_EllipseRGB.clone();
		//cv::Mat showEllipse;
		//cv::flip(m_EllipseRGB, showEllipse, 1);
		//cv::Point2f rotationCenter(m_EllipseRGB.cols / 2.0f, m_EllipseRGB.rows / 2.0f);
		//cv::Mat rot_mat = cv::getRotationMatrix2D(rotationCenter, 180.0f, 1.0);
		//cv::warpAffine(showEllipse, showEllipse, rot_mat, m_EllipseRGB.size());

		cv::imshow(m_ConstWindowName, showEllipse);
	}

	void SLPointToEllipse::DrawPoint()
	{
		m_RandomPoint = cv::Point(m_RandomPoint_x, m_RandomPoint_y);
		cv::circle(m_EllipseRGB, m_RandomPoint, 10, slopencv::CV_COLOR_SCALAR_BLUE, CV_FILLED, 8);
	}

	void SLPointToEllipse::DrawDistanceCircle()
	{
		cv::circle(m_EllipseRGB, m_RandomPoint, (int)m_Distance, slopencv::CV_COLOR_SCALAR_BLACK, 3, cv::LINE_8);
	}
	void SLPointToEllipse::PaintWidgetCallBack(int /*pos*/, void* /*userData*/)
	{
		DrawBasicEllipse();
		DrawPoint();

		m_Ellipse.center = cv::Size2f((float)m_Ellipse_x0, (float)m_Ellipse_y0);
		m_Distance = slopencv::GetDistanceFromPointToEllipse(cv::Point(m_RandomPoint_x, m_RandomPoint_y),
										m_Ellipse, slutil::POINT_TO_ELLIPSE_INTERATIVE_CRITERION_IN_PIXELSQUARE);

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

		cv::createTrackbar("a", m_ConstWindowName, &m_Ellipse_a, WIDGET_SIZE_WIDTH, FunPtrPaintEllipseWidgetCallBack);
		cv::createTrackbar("b", m_ConstWindowName, &m_Ellipse_b, WIDGET_SIZE_WIDTH, FunPtrPaintEllipseWidgetCallBack);
		cv::createTrackbar("theta", m_ConstWindowName, &m_iAngle, 720, FunPtrPaintEllipseWidgetCallBack);
		cv::createTrackbar("x_point", m_ConstWindowName, &m_RandomPoint_x, WIDGET_SIZE_WIDTH, FunPtrPaintEllipseWidgetCallBack);
		cv::createTrackbar("y_point", m_ConstWindowName, &m_RandomPoint_y, WIDGET_SIZE_WIDTH, FunPtrPaintEllipseWidgetCallBack);

		InitialBasicEllipse();

		PaintWidgetCallBack();
		cv::waitKey();
	}



	void SLPointToEllipse::DrawBasicEllipse()
	{
		m_EllipseRGB = cv::Mat(WIDGET_SIZE_WIDTH, WIDGET_SIZE_HEIGHT, CV_8UC3, slopencv::CV_COLOR_SCALAR_EyeProtection);
		m_Ellipse.center = cv::Size2f((float)m_Ellipse_x0, (float)m_Ellipse_y0);

		m_Ellipse.angle = m_iAngle * 1.0f;
		m_Ellipse.size = cv::Size(m_Ellipse_a * 2, m_Ellipse_b * 2);

		cv::ellipse(m_EllipseRGB, m_Ellipse, slopencv::CV_COLOR_SCALAR_BLUE, 3, 8);
	}

	static const int RANDOM_OFFSET = 200;

	void SLPointToEllipse::Samples_CaculateDistanceFromPointToEllipse()
	{
		cv::RotatedRect randomEllipse;
		randomEllipse.center = cv::Size2f(300.0f, 300.0f);
		randomEllipse.angle = 150.0f;
		randomEllipse.size = cv::Size2f(270.0f * 2.0f, 160.0f * 2.0f);

		cv::Point ellipseCenter = { (int)randomEllipse.center.x, (int)randomEllipse.center.y};
		std::vector<cv::Point> randomPointsOffEllipse = {
			{ ellipseCenter.x, ellipseCenter.y },
			// random points in four Quadrant corresponds to Ellipse
			{ ellipseCenter.x + RANDOM_OFFSET, ellipseCenter.y + RANDOM_OFFSET },
			{ ellipseCenter.x + RANDOM_OFFSET, ellipseCenter.y - RANDOM_OFFSET },
			{ ellipseCenter.x - RANDOM_OFFSET, ellipseCenter.y + RANDOM_OFFSET },
			{ ellipseCenter.x - RANDOM_OFFSET, ellipseCenter.y - RANDOM_OFFSET },
		};

		std::vector<double> distances;
		slopencv::GetDistancesArrayFromPointsToEllipse(randomPointsOffEllipse, randomEllipse, 
						slutil::POINT_TO_ELLIPSE_INTERATIVE_CRITERION_IN_PIXELSQUARE, distances);

		std::cout << "Distance to Ellipse, point Ellipse Center: \t" << distances[0];
		std::cout << "\nDistance to Ellipse, point RANDOM_OFFSET in 1st Quadrant: \t" << distances[1];
		std::cout << "\nDistance to Ellipse, point RANDOM_OFFSET in 2nd Quadrant: \t" << distances[2];
		std::cout << "\nDistance to Ellipse, point RANDOM_OFFSET in 3rd Quadrant: \t" << distances[3];
		std::cout << "\nDistance to Ellipse, point RANDOM_OFFSET in 4th Quadrant: \t" << distances[4];
	}



}// End of namespace slopencv

