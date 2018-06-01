#ifndef __SLOPENCVBASICS__
#define __SLOPENCVBASICS__
#pragma once

namespace slopencv
{
	// slutil, statistic
	double GetRootMeanSquare(const std::vector<double>& dataVect);

	extern void GetImageMat(CImage& src, cv::Mat& dst);
	extern double IteratePhiForShortestDistanceToEllipse(double x, double y, double a, double b, double phi = 0.0);
	extern double GetDistanceFromPointToPoint(double x1, double y1, double x2, double y2);
	extern void GetPointRelativeToEllipse(int x_RandomPoint, int y_RandomPoint, double& x_RelativePoint, double& y_RelativePoint
		, double x_EllipseCenter, double y_EllipseCenter, double theta);
	double GetShortestDistanceFromPointToEllipse(const cv::Point& randomPoint, const cv::RotatedRect& ellipse);
	extern double GetExtractedEllipseRootMeanSquare(const std::vector<cv::Point>& ellipseEdges, const cv::RotatedRect& bestFitEllipse);

}

#endif