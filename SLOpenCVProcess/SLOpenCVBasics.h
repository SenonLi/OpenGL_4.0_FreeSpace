#ifndef __SLOPENCVBASICS__
#define __SLOPENCVBASICS__
#pragma once

namespace slopencv
{
	//========================================================
	//--------  slutil::ResolutionValues (namespace)  --------
	static const double MAX_POSITION = 10000.0;

	//--------  slutil::ResolutionValues (namespace)  --------
	//========================================================
	std::string ws2s(const std::wstring& wstr);
	std::wstring s2ws(const std::string& str);

	static const int CV_MINIMUM_NUM_FOR_FIT_ELLIPSE = 5;

	// slutil, statistic
	double GetRootMeanSquare(const std::vector<double>& dataVect);

	void GetImageMat(CImage& src, cv::Mat& dst);
	double IteratePhiForShortestDistanceToEllipse(double x, double y, double a, double b, double phi = 0.0);
	double GetDistanceFromPointToPoint(double x1, double y1, double x2, double y2);
	void GetPointRelativeToEllipse(int x_RandomPoint, int y_RandomPoint, double& x_RelativePoint, double& y_RelativePoint
		, double x_EllipseCenter, double y_EllipseCenter, double theta);
	double GetShortestDistanceFromPointToEllipse(const cv::Point& randomPoint, const cv::RotatedRect& ellipse, double resolution = 0.1);
	double GetExtractedEllipseRootMeanSquare(const std::vector<cv::Point>& ellipseEdges, const cv::RotatedRect& bestFitEllipse);

}

#endif