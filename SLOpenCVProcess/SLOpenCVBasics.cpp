#include "stdafx.h"
#include "SLOpenCVBasics.h"

namespace slopencv
{
	/// <summary>Bad solution to Get cv::Mat from CImage</summary>
	/// <param name="src">CImage [IN]</param>
	/// <param name="dst">cv::Mat [OUT]</param>
	void GetImageMat(CImage& src, cv::Mat& dst)
	{
		assert(!src.IsNull());
		dst.release();

		switch (src.GetBPP())
		{
		case 8:
		{
			dst.create(src.GetHeight(), src.GetWidth(), CV_8UC1);
			for (int row = 0; row < src.GetHeight(); row++)
			{
				for (int col = 0; col < src.GetWidth(); col++)
				{
					dst.at<unsigned char>(row, col) = static_cast<BYTE*>(src.GetPixelAddress(col, row))[0];
				}
			}
			break;
		}

		case 24:
		{
			dst.create(src.GetHeight(), src.GetWidth(), CV_8UC3);
			for (int row = 0; row < src.GetHeight(); row++)
			{
				for (int col = 0; col < src.GetWidth(); col++)
				{
					dst.at<cv::Vec3b>(row, col)[0] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[0];
					dst.at<cv::Vec3b>(row, col)[1] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[1];
					dst.at<cv::Vec3b>(row, col)[2] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[2];
				}
			}

			break;
		}

		case 32:
		{
			dst.create(src.GetHeight(), src.GetWidth(), CV_8UC4);
			for (int row = 0; row < src.GetHeight(); row++)
			{
				for (int col = 0; col < src.GetWidth(); col++)
				{
					dst.at<cv::Vec4b>(row, col)[0] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[0];
					dst.at<cv::Vec4b>(row, col)[1] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[1];
					dst.at<cv::Vec4b>(row, col)[2] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[2];
					dst.at<cv::Vec4b>(row, col)[3] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[3];
				}
			}

			break;
		}

		}


	}



}