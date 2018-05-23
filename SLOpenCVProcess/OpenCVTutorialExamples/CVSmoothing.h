#ifndef __CVSMOOTHING__
#define __CVSMOOTHING__
#pragma once

class CVSmoothing
{
public:
	int DELAY_CAPTION = 1500;
	int DELAY_BLUR = 100;
	int MAX_KERNEL_LENGTH = 31;

	void ShowWidget();

private:
	int display_caption(const char* caption);
	int display_dst(int delay);

	const char* m_WindowName = "Smoothing";
	cv::Mat src;
	cv::Mat dst;

};


#endif // __CVSMOOTHING__
