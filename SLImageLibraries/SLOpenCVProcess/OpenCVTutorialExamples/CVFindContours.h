#ifndef __CVFINDCONTOURS__
#define __CVFINDCONTOURS__
#pragma once

#include "opencv2/highgui.hpp" // For Instancing cv::Mat

/// <summary>Find contours in an image </summary>
class CVFindContours
{
public:
	void ThreshCallBack(int pos = 0, void* userData = nullptr); // Signature here for Trackerbar registration
	void ShowWidget();

private:
	cv::Mat m_Src; 
	cv::Mat m_SrcGray;
	int m_ThreshValue = 100;
	int max_thresh = 255;
	cv::RNG rng = 12345;

};


#endif // __CVFINDCONTOURS__