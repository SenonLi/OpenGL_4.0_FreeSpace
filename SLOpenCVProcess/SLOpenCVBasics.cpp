#include "stdafx.h"
#include "SLOpenCVBasics.h"

namespace slopencv
{
	double IteratePhiForShortestDistanceToEllipse(double x, double y, double a, double b, double phi)
	{
		return atan(((a*a - b * b) * sin(phi) + abs(y) * b) / (abs(x) * a));
	}

	double GetDistanceFromPointToPoint(double x1, double y1, double x2, double y2)
	{
		return sqrt( (x1 - x2) * (x1 - x2) + (y1 -y2) * (y1 - y2) );
	}

	void GetPointRelativeToEllipse(int x_RandomPoint, int y_RandomPoint, double& x_RelativePoint, double& y_RelativePoint
		, double x_EllipseCenter, double y_EllipseCenter, double theta)
	{
		x_RelativePoint = abs( (x_RandomPoint - x_EllipseCenter) * cos(theta) + (y_RandomPoint - y_EllipseCenter) * sin(theta) );
		y_RelativePoint = abs( -(x_RandomPoint - x_EllipseCenter) * sin(theta) + (y_RandomPoint - y_EllipseCenter) * cos(theta) );
	}

	/// <summary>Bad solution to Get cv::Mat from CImage</summary>
	/// <param name="src">CImage [IN]</param>
	/// <param name="dst">cv::Mat [OUT]</param>
	void GetImageMat(CImage& src, cv::Mat& dst)
	{
		assert(!src.IsNull());
		dst.release();

		switch (src.GetBPP())
		{
		case 8:
		{
			dst.create(src.GetHeight(), src.GetWidth(), CV_8UC1);
			for (int row = 0; row < src.GetHeight(); row++)
			{
				for (int col = 0; col < src.GetWidth(); col++)
				{
					dst.at<unsigned char>(row, col) = static_cast<BYTE*>(src.GetPixelAddress(col, row))[0];
				}
			}
			break;
		}

		case 24:
		{
			dst.create(src.GetHeight(), src.GetWidth(), CV_8UC3);
			for (int row = 0; row < src.GetHeight(); row++)
			{
				for (int col = 0; col < src.GetWidth(); col++)
				{
					dst.at<cv::Vec3b>(row, col)[0] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[0];
					dst.at<cv::Vec3b>(row, col)[1] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[1];
					dst.at<cv::Vec3b>(row, col)[2] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[2];
				}
			}

			break;
		}

		case 32:
		{
			dst.create(src.GetHeight(), src.GetWidth(), CV_8UC4);
			for (int row = 0; row < src.GetHeight(); row++)
			{
				for (int col = 0; col < src.GetWidth(); col++)
				{
					dst.at<cv::Vec4b>(row, col)[0] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[0];
					dst.at<cv::Vec4b>(row, col)[1] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[1];
					dst.at<cv::Vec4b>(row, col)[2] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[2];
					dst.at<cv::Vec4b>(row, col)[3] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[3];
				}
			}

			break;
		}

		}


	}



}