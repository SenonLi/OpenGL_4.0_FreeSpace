#ifndef __SLOPENCVPROCESS__
#define __SLOPENCVPROCESS__
#pragma once

#include <vector>

namespace slopencv
{
	static const cv::Scalar CV_COLOR_SCALAR_RED = cv::Scalar(0, 0, 255);
	static const cv::Scalar CV_COLOR_SCALAR_BLUE = cv::Scalar(255, 0, 0);
	static const cv::Scalar CV_COLOR_SCALAR_YELLOW = cv::Scalar(0, 255, 255);
	static const cv::Scalar CV_COLOR_SCALAR_CYAN = cv::Scalar(255, 255, 0);
	static const cv::Scalar CV_COLOR_SCALAR_PURPLE = cv::Scalar(255, 0, 255);
	static const cv::Scalar CV_COLOR_SCALAR_WHITE = cv::Scalar(255, 255, 255);
	static const cv::Scalar CV_COLOR_SCALAR_BLACK = cv::Scalar(0, 0, 0);
	static const cv::Scalar CV_COLOR_SCALAR_EyeProtection = cv::Scalar(187, 223, 187);

	static const int CV_MINIMUM_NUM_FOR_FIT_ELLIPSE = 5;

	std::string ws2s(const std::wstring& wstr);
	std::wstring s2ws(const std::string& str);

	// slutil, statistic
	double GetRootMeanSquare(const std::vector<double>& dataVect);

	double IteratePhiForShortestDistanceToEllipse(double x, double y, double a, double b, double phi, int interationCount = 0);
	void GetPointRelativeToEllipse(const cv::Point2d& randomPoint, const cv::Point2d& targetEllipseCenter
		, double sinTheta, double cosTheta, cv::Point2d& relativePoint);
	
	double GetDistanceFromPointToPoint(double x1, double y1, double x2, double y2);
	double GetDistanceFromPointToEllipse(const cv::Point& randomPoint, const cv::RotatedRect& ellipse, double iterativeCriterion);
	void GetDistancesArrayFromPointsToEllipse(const std::vector<cv::Point>& randomPointsVect, const cv::RotatedRect& targetEllipse, double iterativeCriterion, std::vector<double>& distancesVect);

	double GetExtractedEllipseRootMeanSquare(const std::vector<cv::Point>& ellipseEdges, const cv::RotatedRect& bestFitEllipse);

}

#endif // __SLOPENCVPROCESS__ 