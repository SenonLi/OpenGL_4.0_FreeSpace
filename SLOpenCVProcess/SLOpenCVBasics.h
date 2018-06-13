#ifndef __SLOPENCVBASICS__
#define __SLOPENCVBASICS__
#pragma once

namespace slopencv
{
	//========================================================
	//--------  slutil::ResolutionValues (namespace)  --------
	static const double MAX_POSITION = 10000.0;
	static const double POINT_TO_ELLIPSE_INTERATIVE_CRITERION_IN_PIXELSQUARE = 0.1;
	static const double POINT_TO_ELLIPSE_INTERATIVE_MIN_PHI_IN_PIXEL = CV_PI / 10.0;

	//--------  slutil::ResolutionValues (namespace)  --------
	//========================================================
	std::string ws2s(const std::wstring& wstr);
	std::wstring s2ws(const std::string& str);

	static const int CV_MINIMUM_NUM_FOR_FIT_ELLIPSE = 5;
	static const double INVALID_ELLIPSE_EXTRACTION_RMS = -1.0;

	// slutil, statistic
	double GetRootMeanSquare(const std::vector<double>& dataVect);

	void ConvertImageToCVMat(CImage& src, cv::Mat& dst);
	
	double IteratePhiForShortestDistanceToEllipse(double x, double y, double a, double b, double phi, int interationCount = 0);
	void GetPointRelativeToEllipse(const cv::Point2d& randomPoint, const cv::Point2d& targetEllipseCenter
		, double sinTheta, double cosTheta, cv::Point2d& relativePoint);
	
	double GetDistanceFromPointToPoint(double x1, double y1, double x2, double y2);
	double GetDistanceFromPointToEllipse(const cv::Point& randomPoint, const cv::RotatedRect& ellipse, double iterativeCriterion);
	void GetDistancesArrayFromPointsToEllipse(const std::vector<cv::Point>& randomPointsVect, const cv::RotatedRect& targetEllipse, double iterativeCriterion, std::vector<double>& distancesVect);

	double GetExtractedEllipseRootMeanSquare(const std::vector<cv::Point>& ellipseEdges, const cv::RotatedRect& bestFitEllipse);

}

#endif