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
		
		void DetermineShortestDistanceFromPointToEllipse();


		void DrawDistanceCircle();
		void PaintScreen();

		int m_ImageFlags = cv::WINDOW_NORMAL;
		const char* m_ConstWindowName = "Point To Ellipse";

		const int WIDGET_SIZE_WIDTH = 600;
		const int WIDGET_SIZE_HEIGHT = 600;

		const int m_Ellipse_a = WIDGET_SIZE_WIDTH * 6 / 16;
		const int m_Ellipse_b = WIDGET_SIZE_HEIGHT * 3 / 16;
		
		int m_Ellipse_x0 = WIDGET_SIZE_WIDTH * 2 / 5;
		int m_Ellipse_y0 = WIDGET_SIZE_HEIGHT * 3 / 5;
		int m_RandomPoint_x = WIDGET_SIZE_WIDTH * 15 / 16;
		int m_RandomPoint_y = WIDGET_SIZE_HEIGHT * 15 / 16;
		double m_Distance = 100.0;

		cv::Mat m_EllipseRGB;
		cv::RotatedRect m_Ellipse;
		cv::Point m_RandomPoint;
		const float Ellipse_Angle = 60.0f; // in degree

	};


}// End of namespace slopencv
#endif // __SLPOINTTOELLIPSE__