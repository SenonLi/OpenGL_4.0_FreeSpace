#include "stdafx.h"
#include "SLBinaryEllipseCorrelation.h"
#include <iostream>

namespace slopencv
{

	int SLBinaryEllipseCorrelation::display_caption(const char* caption)
	{
		m_Dst = cv::Mat::zeros(m_Src.size(), m_Src.type());
		cv::putText(m_Dst, caption,
			cv::Point(m_Src.cols / 4, m_Src.rows / 2),
			cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255));

		return display_dst("", DELAY_CAPTION);
	}

	int SLBinaryEllipseCorrelation::display_dst(const char* windowName, int delay)
	{
		cv::imshow(windowName, m_Dst);
		int c = cv::waitKey(delay);
		if (c >= 0) { return -1; }
		return 0;
	}

	void SLBinaryEllipseCorrelation::ShowGaussianAdaptiveThresholding()
	{
		/// Applying Gaussian blur
		for (int i = 15; i < MAX_KERNEL_LENGTH; i = i + 2)
		{
			cv::GaussianBlur(m_Src, m_Dst, cv::Size(i, i), 0, 0);
			display_dst("Gaussian Blur", DELAY_BLUR);
			cv::moveWindow("Gaussian Blur", m_Dst.cols + 350, 170);

			m_Dst = m_Src - m_Dst;
			display_dst("Adaptive Thresholding", DELAY_BLUR);
			cv::moveWindow("Adaptive Thresholding", m_Dst.cols + 350, m_Dst.rows + 200);
		}
	}


	void SLBinaryEllipseCorrelation::ShowWidget()
	{
		m_Src = cv::imread(m_FileName, cv::IMREAD_GRAYSCALE);
		assert(!m_Src.empty());

		m_Dst = m_Src.clone();
		cv::imshow("Original", m_Dst);
		cv::moveWindow("Original", 300, 270);

		ShowGaussianAdaptiveThresholding();
		
		cv::waitKey();
	}




} // End of namespace slopencv


//#include "OpenCVTutorialExamples/CVThreshold.h"
#include "OpenCVTutorialExamples/CVSmoothing.h"
#include "OpenCVTutorialExamples\CVShapeExample.h"

using namespace slopencv;

int main(int argc, char *argv[])
{
	SLBinaryEllipseCorrelation cameraWidget;
  	cameraWidget.ShowWidget();
  
  	return 1;
}