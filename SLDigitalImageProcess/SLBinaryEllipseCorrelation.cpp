#include "stdafx.h"
#include "SLBinaryEllipseCorrelation.h"
#include <iostream>

namespace slopencv
{

	int SLBinaryEllipseCorrelation::display_caption(const char* caption)
	{
		dst = cv::Mat::zeros(src.size(), src.type());
		cv::putText(dst, caption,
			cv::Point(src.cols / 4, src.rows / 2),
			cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255));

		return display_dst(DELAY_CAPTION);
	}

	int SLBinaryEllipseCorrelation::display_dst(int delay)
	{
		cv::imshow(window_name, dst);
		int c = cv::waitKey(delay);
		if (c >= 0) { return -1; }
		return 0;
	}


	int SLBinaryEllipseCorrelation::showWidget()
	{
		cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
		const char* filename = "../WatchMe/Images/Einstein.jpg";

		src = cv::imread(filename, cv::IMREAD_COLOR);
		assert(!src.empty());

		if (display_caption("Original Image") != 0) { return 0; }
		dst = src.clone();
		if (display_dst(DELAY_CAPTION) != 0) { return 0; }

		/// Applying Gaussian blur
		if (display_caption("Gaussian Blur") != 0) { return 0; }
		//![gaussianblur]
		for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
		{
			cv::GaussianBlur(src, dst, cv::Size(i, i), 0, 0);
			if (display_dst(DELAY_BLUR) != 0) { return 0; }
		}

		/// Applying Bilateral Filter
		if (display_caption("Bilateral Blur") != 0) { return 0; }
		for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
		{
			bilateralFilter(src, dst, i, i * 2, i / 2);
			if (display_dst(DELAY_BLUR) != 0) { return 0; }
		}

		/// Done
		display_caption("Done!");

		return 0;
	}


} // End of namespace slopencv


//#include "OpenCVTutorialExamples/CVThreshold.h"
#include "OpenCVTutorialExamples/CVSmoothing.h"
#include "OpenCVTutorialExamples\CVShapeExample.h"

using namespace slopencv;

int main(int argc, char *argv[])
{
	CVShapeExample cameraWidget;
  	cameraWidget.showWidget(argc, argv);
  
  	return 1;
}