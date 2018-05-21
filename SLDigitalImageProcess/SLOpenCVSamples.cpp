#include "stdafx.h"
#include "SLOpenCVSamples.h"

namespace slopencv
{
	/// <summary>Sample code that shows how to use the diverse threshold options offered by OpenCV</summary>
	void SLOpenCVSamples::Test_Threshold()
	{
	}
} // End of namespace slopencv


#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


	/// Global Variables
	int DELAY_CAPTION = 1500;
	int DELAY_BLUR = 100;
	int MAX_KERNEL_LENGTH = 31;

	cv::Mat src; 
	cv::Mat dst;
	char window_name[] = "Smoothing Demo";

	/// Function headers
	int display_caption(const char* caption);
	int display_dst(int delay);


	/**
	* @function display_caption
	*/
	int display_caption(const char* caption)
	{
		dst = cv::Mat::zeros(src.size(), src.type());
		putText(dst, caption,
			cv::Point(src.cols / 4, src.rows / 2),
			cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255));

		return display_dst(DELAY_CAPTION);
	}

	/**
	* @function display_dst
	*/
	int display_dst(int delay)
	{
		cv::imshow(window_name, dst);
		int c = cv::waitKey(delay);
		if (c >= 0) { return -1; }
		return 0;
	}



  /**
  * function main
  */
int main(int argc, char ** argv)
{
	cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);

	/// Load the source image
	const char* filename = argc >= 2 ? argv[1] : "../WatchMe/Images/poor_3.bmp";

	src = cv::imread(filename, cv::IMREAD_COLOR);
	assert(!src.empty());

	if (display_caption("Original Image") != 0) { return 0; }

	dst = src.clone();
	if (display_dst(DELAY_CAPTION) != 0) { return 0; }


	/// Applying Homogeneous blur
	if (display_caption("Homogeneous Blur") != 0) { return 0; }

	//![blur]
	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		cv::blur(src, dst, cv::Size(i, i), cv::Point(-1, -1));
		if (display_dst(DELAY_BLUR) != 0) { return 0; }
	}
	//![blur]

	/// Applying Gaussian blur
	if (display_caption("Gaussian Blur") != 0) { return 0; }

	//![gaussianblur]
	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		cv::GaussianBlur(src, dst, cv::Size(i, i), 0, 0);
		if (display_dst(DELAY_BLUR) != 0) { return 0; }
	}
	//![gaussianblur]

	/// Applying Median blur
	if (display_caption("Median Blur") != 0) { return 0; }

	//![medianblur]
	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		medianBlur(src, dst, i);
		if (display_dst(DELAY_BLUR) != 0) { return 0; }
	}
	//![medianblur]

	/// Applying Bilateral Filter
	if (display_caption("Bilateral Blur") != 0) { return 0; }

	//![bilateralfilter]
	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		bilateralFilter(src, dst, i, i * 2, i / 2);
		if (display_dst(DELAY_BLUR) != 0) { return 0; }
	}
	//![bilateralfilter]

	/// Done
	display_caption("Done!");

	return 0;
}