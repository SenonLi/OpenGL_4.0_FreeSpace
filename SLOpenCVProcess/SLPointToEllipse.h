#ifndef __SLPOINTTOELLIPSE__
#define __SLPOINTTOELLIPSE__
#pragma once

#include "opencv2/highgui.hpp" // For Instancing cv::Mat

namespace slopencv
{
	class SLPointToEllipse
	{
	public:
		void ShowWidget();
		void PaintWidgetCallBack(int pos = 0, void* userData = nullptr); // Signature here for Trackerbar registration

	private:
		void InitialBasicEllipse();
		void DrawBasicEllipse();
		void DrawPoint();
		
		
		void CalculateDistance();


		void DrawDistanceCircle();
		void PaintScreen();

		int m_ImageFlags = cv::WINDOW_NORMAL;
		const char* m_ConstWindowName = "Point To Ellipse";

		int m_Ellipse_a = 600;
		int m_Ellipse_b = 400;
		int m_Point_x = 600;
		int m_Point_y = 400;
		double m_Distance = 100.0;

		cv::Mat m_EllipseRGB;
		cv::RotatedRect m_Ellipse;
		cv::Point m_RandomPoint = cv::Point(600, 400);
		const cv::Point2f Ellipse_Center = cv::Point2f(0.0f, 0.0f);
		const cv::Size2f Ellipse_Size = cv::Size2f(800.0f, 800.0f);
		const float Ellipse_Angle = 0.0f;

	};


}// End of namespace slopencv
#endif // __SLPOINTTOELLIPSE__