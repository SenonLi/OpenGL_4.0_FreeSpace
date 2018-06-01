#include "stdafx.h"
#include "SLOpenCVBasics.h"

namespace slopencv
{
	// slutil, statistic
	double GetRootMeanSquare(const std::vector<double> dataVect)
	{
		double sum = 0.0;
		for (int i = 0; i < dataVect.size(); ++i) {
			sum += dataVect[i] * dataVect[i];
		}
		return sqrt(sum / dataVect.size());
	}

	// 	slgeom
	double GetDistanceFromPointToPoint(double x1, double y1, double x2, double y2)
	{
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}

	double IteratePhiForShortestDistanceToEllipse(double x, double y, double a, double b, double phi)
	{
		return atan(((a*a - b * b) * sin(phi) + abs(y) * b) / (abs(x) * a));
	}

	void GetPointRelativeToEllipse(int x_RandomPoint, int y_RandomPoint, double& x_RelativePoint, double& y_RelativePoint
		, double x_EllipseCenter, double y_EllipseCenter, double theta)
	{
		x_RelativePoint = abs( (x_RandomPoint - x_EllipseCenter) * cos(theta) + (y_RandomPoint - y_EllipseCenter) * sin(theta) );
		y_RelativePoint = abs( -(x_RandomPoint - x_EllipseCenter) * sin(theta) + (y_RandomPoint - y_EllipseCenter) * cos(theta) );
	}

	double GetShortestDistanceFromPointToEllipse(const cv::Point& randomPoint, const cv::RotatedRect& ellipse)
	{
		double phiShortest = CV_PI / 2.0; // starting point has to be 90 degree, if start from 0, iteration may return two same values due to sin(0) == 0
		double shortestDistance = 999999.0;
		double newDistance = shortestDistance - 2.0;
		randomPoint.x;
		ellipse.angle;
		ellipse.center.x;
			double majorAxisLength = ellipse.size.width / 2.0;
		double minorAxisLength = ellipse.size.height / 2.0;

		double x_RelativePoint = -1.0;
		double y_RelativePoint = -1.0;
		slopencv::GetPointRelativeToEllipse(randomPoint.x, randomPoint.y, x_RelativePoint, y_RelativePoint, ellipse.center.x, ellipse.center.y, ellipse.angle / 180.0 * CV_PI);
		int iterationCount = 0;

		while (shortestDistance - newDistance > 1)
		{
			shortestDistance = newDistance;
			phiShortest = slopencv::IteratePhiForShortestDistanceToEllipse(x_RelativePoint, y_RelativePoint, majorAxisLength, minorAxisLength, phiShortest);
			newDistance = slopencv::GetDistanceFromPointToPoint(abs(x_RelativePoint), abs(y_RelativePoint), majorAxisLength * cos(phiShortest), minorAxisLength * sin(phiShortest));
			iterationCount++;
		}

		return shortestDistance;
	}


	double GetExtractedEllipseRootMeanSquare(const std::vector<cv::Point>& ellipseEdges, const cv::RotatedRect& bestFitEllipse)
	{
		if (ellipseEdges.size() <= 0)
			return -1.0;

		std::vector<double> edgesDistancesToBestFitEllipse;
		edgesDistancesToBestFitEllipse.resize(ellipseEdges.size());

		for (int i = 0; i < ellipseEdges.size(); ++i)
		{
			edgesDistancesToBestFitEllipse[i] = GetShortestDistanceFromPointToEllipse(ellipseEdges[i], bestFitEllipse);
		}

		return GetRootMeanSquare(edgesDistancesToBestFitEllipse);
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