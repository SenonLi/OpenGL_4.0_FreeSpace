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
		double semiMajor = m_Ellipse_a;
		double semiMinor = m_Ellipse_b;
		double thetaInRadian = Ellipse_Angle / 180.0 * CV_PI;
		double sinTheta = sin(thetaInRadian);
		double cosTheta = cos(thetaInRadian);

		cv::Point2d relativePoint;
		slopencv::GetPointRelativeToEllipse(cv::Point(m_RandomPoint_x, m_RandomPoint_y), cv::Point(m_Ellipse_x0, m_Ellipse_y0), sinTheta, cosTheta, relativePoint);

		double shortestDistance = slopencv::MAX_POSITION;
		double newDistance = shortestDistance - 2.0;

		// phiShortest (phi) here is the angle start from semi-Major-Axis of random ellipse, to the intersection point ray 
		// and the ray starts from center of ellipse to the intersection point on elllipse, which is the closest point to the random point on the ellipse
		double phiShortest = atan2(relativePoint.y, relativePoint.x);
		// Make sure the initial phiShortest is not too small, in case slopencv::IteratePhiForShortestDistanceToEllipse won't work well (shortestDistance would be to close to newDistance )
		if (phiShortest < POINT_TO_ELLIPSE_INTERATIVE_MIN_PHI_IN_PIXEL)
			phiShortest = POINT_TO_ELLIPSE_INTERATIVE_MIN_PHI_IN_PIXEL;

		int iterationCount = 0;
		while (shortestDistance - newDistance > POINT_TO_ELLIPSE_INTERATIVE_CRITERION_IN_PIXEL)
		{
			shortestDistance = newDistance;
			phiShortest = slopencv::IteratePhiForShortestDistanceToEllipse(relativePoint.x, relativePoint.y, semiMajor, semiMinor, phiShortest);
			newDistance = slopencv::GetDistanceFromPointToPoint(abs(relativePoint.x), abs(relativePoint.y), semiMajor * cos(phiShortest), semiMinor * sin(phiShortest));
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
		m_Distance = slopencv::GetDistanceFromPointToEllipse(cv::Point(m_RandomPoint_x, m_RandomPoint_y), m_Ellipse, POINT_TO_ELLIPSE_INTERATIVE_CRITERION_IN_PIXEL);

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






}// End of namespace slopencv

