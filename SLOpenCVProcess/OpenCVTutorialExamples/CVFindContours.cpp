#include "../stdafx.h"
#include "CVFindContours.h"

CVFindContours* ptrCVFindContoursInstance;

extern "C" void FunPtrThreshCallBack(int pos, void* userData)
{
	ptrCVFindContoursInstance->ThreshCallBack(pos, userData);
}

void CVFindContours::ShowWidget()
{
	::ptrCVFindContoursInstance = this;

	/// Load source image
	cv::String imageName("../WatchMe/Images/OpenCV_Samples//happyfish.jpg");
	m_Src = cv::imread(imageName, cv::IMREAD_COLOR);
	assert(!m_Src.empty());

	/// Convert image to gray and blur it
	cv::cvtColor(m_Src, m_SrcGray, cv::COLOR_BGR2GRAY);
	cv::blur(m_SrcGray, m_SrcGray, cv::Size(3, 3));

	/// Create Window
	const char* source_window = "Source";
	cv::namedWindow(source_window, cv::WINDOW_AUTOSIZE);
	cv::imshow(source_window, m_Src);

	cv::createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, FunPtrThreshCallBack);
	ThreshCallBack(0, nullptr);

	cv::waitKey(0);
}

void CVFindContours::ThreshCallBack(int /*pos*/, void* /*userData*/)
{
	cv::Mat canny_output;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	/// Detect edges using canny
	cv::Canny(m_SrcGray, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	cv::findContours(canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	/// Draw contours
	cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i< contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, cv::Point());
	}

	/// Show in a window
	cv::namedWindow("Contours", cv::WINDOW_AUTOSIZE);
	cv::imshow("Contours", drawing);
}
