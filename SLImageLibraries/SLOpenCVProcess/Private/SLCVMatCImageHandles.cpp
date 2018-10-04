#include "../stdafx.h"

#include "SLCVMatCImageHandles.h"
#include "StaticConstBasics\SLGeneralImageBasics.h"

namespace slopencv
{

	/// <summary>Bad solution to Get cv::Mat from CImage</summary>
	/// <param name="src">CImage [IN]</param>
	/// <param name="dst">cv::Mat [OUT]</param>
	void ConvertCImageToCVMat(CImage& src, cv::Mat& dst)
	{
		assert(!src.IsNull());
		dst.release();

		switch (src.GetBPP())
		{
		case 8:
			dst.create(src.GetHeight(), src.GetWidth(), CV_8UC1);
			for (int row = 0; row < src.GetHeight(); row++) {
				for (int col = 0; col < src.GetWidth(); col++) {
					dst.at<unsigned char>(row, col) = static_cast<BYTE*>(src.GetPixelAddress(col, row))[0];
				}
			}
			break;

		case 24:
			dst.create(src.GetHeight(), src.GetWidth(), CV_8UC3);
			for (int row = 0; row < src.GetHeight(); row++) {
				for (int col = 0; col < src.GetWidth(); col++) {
					dst.at<cv::Vec3b>(row, col)[0] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[0];
					dst.at<cv::Vec3b>(row, col)[1] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[1];
					dst.at<cv::Vec3b>(row, col)[2] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[2];
				}
			}
			break;

		case 32:
			dst.create(src.GetHeight(), src.GetWidth(), CV_8UC4);
			for (int row = 0; row < src.GetHeight(); row++) {
				for (int col = 0; col < src.GetWidth(); col++) {
					dst.at<cv::Vec4b>(row, col)[0] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[0];
					dst.at<cv::Vec4b>(row, col)[1] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[1];
					dst.at<cv::Vec4b>(row, col)[2] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[2];
					dst.at<cv::Vec4b>(row, col)[3] = static_cast<BYTE*>(src.GetPixelAddress(col, row))[3];
				}
			}
			break;

		}// End of switch (src.GetBPP())
	}// End of ConvertCImageToCVMat(CImage& src, cv::Mat& dst)




	 /// <summary>cv::imwrite doesn't support Unicode, we need to use CImage to save </summary>
	 /// <param name="in">cv::Mat image [IN]</param>
	 /// <param name="out">CImage image [OUT]</param>
	 /// <remarks>This function only accept BYTE image (8bits per single channel), doesn't support double value image </remarks>
	void ConvertCVMatToCImage(const cv::Mat& in, CImage& out)
	{
		assert(!in.empty());
		assert(in.depth() == CV_8U); // Only support BYTE image
		if (!out.IsNull())
			out.Destroy();
		if (in.type() == CV_8UC1) // single channel
			out.Create(in.cols, in.rows, in.channels() * slutil::BYTE_IMAGE_SINGLE_CHANNEL_BITS * 3);//CImage doesn't support 8bit single pixel color change
		else if (in.type() == CV_8UC4)
			out.Create(in.cols, in.rows, in.channels() * slutil::BYTE_IMAGE_SINGLE_CHANNEL_BITS, CImage::createAlphaChannel);
		else
			out.Create(in.cols, in.rows, in.channels() * slutil::BYTE_IMAGE_SINGLE_CHANNEL_BITS);

		switch (in.channels())
		{
		case 1:
		{
			for (int row = 0; row < in.rows; row++) {
				for (int col = 0; col < in.cols; col++) {
					BYTE intensity = in.at<BYTE>(row, col);
					static_cast<BYTE*>(out.GetPixelAddress(col, row))[0] = intensity;
					static_cast<BYTE*>(out.GetPixelAddress(col, row))[1] = intensity;
					static_cast<BYTE*>(out.GetPixelAddress(col, row))[2] = intensity;
				}
			}
		}break;

		case 3:
		{
			for (int row = 0; row < in.rows; row++) {
				for (int col = 0; col < in.cols; col++) {
					static_cast<BYTE*>(out.GetPixelAddress(col, row))[0] = in.at<cv::Vec3b>(row, col)[0];
					static_cast<BYTE*>(out.GetPixelAddress(col, row))[1] = in.at<cv::Vec3b>(row, col)[1];
					static_cast<BYTE*>(out.GetPixelAddress(col, row))[2] = in.at<cv::Vec3b>(row, col)[2];
				}
			}
		}break;

		case 4:
		{
			for (int row = 0; row < in.rows; row++) {
				for (int col = 0; col < in.cols; col++) {
					static_cast<BYTE*>(out.GetPixelAddress(col, row))[0] = in.at<cv::Vec4b>(row, col)[0];
					static_cast<BYTE*>(out.GetPixelAddress(col, row))[1] = in.at<cv::Vec4b>(row, col)[1];
					static_cast<BYTE*>(out.GetPixelAddress(col, row))[2] = in.at<cv::Vec4b>(row, col)[2];
					static_cast<BYTE*>(out.GetPixelAddress(col, row))[3] = in.at<cv::Vec4b>(row, col)[3];
				}
			}
		}break;

		default:
			assert(true);// Doesn't support cases other than 8bits/24bits/32bits image
		} // End of switch (in.channels())
	}// End of ConvertCVMatToCImage(const cv::Mat& in, CImage& out)

	
	/// <summary>Get down-sampled CImage based on maxWidthInPixel from raw image stream </summary>
	/// <param name="srcEncodedImage">Source Image Stream read from raw image file [IN]</param>
	/// <param name="maxWidthInPixel">Down-sampled image width [IN]</param>
	/// <param name="dstImage">Down-sampled CImage [OUT]</param>
	void GetSmallerImageByWidth(const std::vector<BYTE>& srcEncodedImage, int maxWidthInPixel, CImage& dstImage)
	{
		assert(!srcEncodedImage.empty());

		cv::Mat fullImage = cv::imdecode(cv::Mat(srcEncodedImage), cv::IMREAD_UNCHANGED);
		assert(!fullImage.empty() && fullImage.depth() == CV_8U);

		if (fullImage.type() == CV_8UC1)
			cv::cvtColor(fullImage, fullImage, cv::COLOR_GRAY2RGB);

		if (fullImage.cols > maxWidthInPixel)
		{
			cv::Size newSize(maxWidthInPixel, static_cast<int>(static_cast<double>(fullImage.rows) / fullImage.cols * maxWidthInPixel));
			cv::resize(fullImage, fullImage, newSize, 0, 0, cv::INTER_CUBIC);
		}

		ConvertCVMatToCImage(fullImage, dstImage);
	}// End of GetImageByWidth(const std::vector<BYTE>& srcEncodedImage, int maxWidthInPixel, CImage& dstImage)

} // End of namespace slopencv
