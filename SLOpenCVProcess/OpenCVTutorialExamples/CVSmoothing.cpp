#include "../stdafx.h"
#include "CVSmoothing.h"

#include <iostream>

int CVSmoothing::display_caption(const char* caption)
{
	dst = cv::Mat::zeros(src.size(), src.type());
	cv::putText(dst, caption,
		cv::Point(src.cols / 4, src.rows / 2),
		cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255));

	return display_dst(DELAY_CAPTION);
}

int CVSmoothing::display_dst(int delay)
{
	cv::imshow(m_WindowName, dst);
	int c = cv::waitKey(delay);
	if (c >= 0) { return -1; }
	return 0;
}


void CVSmoothing::ShowWidget()
{
	cv::namedWindow(m_WindowName, cv::WINDOW_AUTOSIZE);
	const char* filename = "../WatchMe/Images/poor_3.bmp";

	src = cv::imread(filename, cv::IMREAD_COLOR);
	assert(!src.empty());

	display_caption("Original Image");

	dst = src.clone();
	display_dst(DELAY_CAPTION);


	/// Applying Homogeneous blur
	display_caption("Homogeneous Blur");
	//![blur]
	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		cv::blur(src, dst, cv::Size(i, i), cv::Point(-1, -1));
		display_dst(DELAY_BLUR);
	}

	/// Applying Gaussian blur
	display_caption("Gaussian Blur");
	//![gaussianblur]
	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		cv::GaussianBlur(src, dst, cv::Size(i, i), 0, 0);
		display_dst(DELAY_BLUR);
	}

	/// Applying Median blur
	display_caption("Median Blur");
	//![medianblur]
	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		medianBlur(src, dst, i);
		display_dst(DELAY_BLUR);
	}

	/// Applying Bilateral Filter
	display_caption("Bilateral Blur");

	//![bilateralfilter]
	for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
	{
		bilateralFilter(src, dst, i, i * 2, i / 2);
		display_dst(DELAY_BLUR);
	}
	//![bilateralfilter]

	/// Done
	display_caption("Done!");
}