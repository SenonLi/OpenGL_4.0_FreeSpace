#ifndef __SLOPENCVBASICS__
#define __SLOPENCVBASICS__
#pragma once

namespace slopencv
{
	extern void GetImageMat(CImage& src, cv::Mat& dst);
	extern double IteratePhiForShortestDistanceToEllipse(double x, double y, double a, double b, double phi = 0.0);

}

#endif