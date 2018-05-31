#include "stdafx.h"
#include "SLPointToEllipse.h"

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
		m_RandomPoint = cv::Point(m_Point_x, m_Point_y);
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

		DrawDistanceCircle();

		PaintScreen();
	}

	void SLPointToEllipse::InitialBasicEllipse()
	{
		m_Ellipse.center = Ellipse_Center;
		m_Ellipse.size = Ellipse_Size;
		m_Ellipse.angle = Ellipse_Angle;
	}

	void SLPointToEllipse::ShowWidget()
	{
		::ptrSLPointToEllipseInstance = this;

		cv::namedWindow(m_ConstWindowName, m_ImageFlags); // Create a window to display results
		cv::resizeWindow(m_ConstWindowName, 800, 1000);

		cv::createTrackbar("a", m_ConstWindowName, &m_Ellipse_a, 800, FunPtrPaintEllipseWidgetCallBack);
		cv::createTrackbar("b", m_ConstWindowName, &m_Ellipse_b, 800, FunPtrPaintEllipseWidgetCallBack);
		cv::createTrackbar("x", m_ConstWindowName, &m_Point_x, 800, FunPtrPaintEllipseWidgetCallBack);
		cv::createTrackbar("y", m_ConstWindowName, &m_Point_y, 800, FunPtrPaintEllipseWidgetCallBack);

		InitialBasicEllipse();

		PaintWidgetCallBack();
		cv::waitKey();
	}



	void SLPointToEllipse::DrawBasicEllipse()
	{
		m_EllipseRGB = cv::Mat(800, 800, CV_8UC3, slopencv::CV_COLOR_SCALAR_EyeProtection);
		m_Ellipse.size = cv::Size(m_Ellipse_a * 2, m_Ellipse_b * 2);
		cv::ellipse(m_EllipseRGB, m_Ellipse, slopencv::CV_COLOR_SCALAR_BLUE, 3, 8);
	}






}// End of namespace slopencv

