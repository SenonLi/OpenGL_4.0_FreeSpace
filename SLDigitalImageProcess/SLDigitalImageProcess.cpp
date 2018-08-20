#include "stdafx.h"
#include "SLDigitalImageProcess.h"

#include "SLLibreImageCImageHandles.h"

namespace sldip
{
	void MeanFilterBlur(SLImageParam& /*textureParam*/)
	{
	}// End of MeanFilterBlur

	void GaussianFilterBlur(SLImageParam& /*textureParam*/)
	{

	}// End of GaussianFilterBlur

	/// <summary>Do Image Historgram Equalization, to enhance image contrast</summary>
	/// <param name="textureParam">All basic Image Info; with Beginning address of image buffer [IN/OUT]</param>
	void AdaptiveThresholding(SLImageParam& textureParam, SLAdaptiveThresholdingType filterType)
	{
		// Copy Image for Background Preparation
		// .....
		// Get Image Background
		switch (filterType)
		{
		case SLAdaptiveThresholdingType::ADAPTIVE_THRESH_MEAN:
			MeanFilterBlur(textureParam);
			break;
		case SLAdaptiveThresholdingType::ADAPTIVE_THRESH_GAUSSIAN:
			GaussianFilterBlur(textureParam);
			break;
		default:
			assert(false);
		}

	}// End of AdaptiveThresholding

	void GetGrascaledImage(const SLLibreImage& srcImage, SLLibreImage& dstImage)
	{
		assert(!srcImage.IsNull() && srcImage.GetColorType() != SLLibreColorType::LibreColorUndefined);
		assert(srcImage.GetBufferEntry() != dstImage.GetBufferEntry());
		// Only support Gray/RGB/BGR/RGBA/BGRA, doesn't support XYZRGB
		assert(srcImage.GetChannels() <= SLImageParam::GetChannelsNum(SLLibreColorType::LibreColorRGBA));
		if (srcImage.GetColorType() == SLLibreColorType::LibreColorGray)
			dstImage = srcImage;
		else 
		{
			dstImage.CreateLibreImage(srcImage.GetWidth(), srcImage.GetHeight(), SLLibreColorType::LibreColorGray);
			assert(!dstImage.IsNull());

			int srcPitch = srcImage.GetPitch();
			int dstPitch = dstImage.GetPitch();

			const BYTE* srcDataEntry = srcImage.GetBufferEntry();
			BYTE* dstDataEntry = dstImage.GetBufferEntryForEdit();
			unsigned int srcChannels = srcImage.GetChannels();
			bool srcHasAlphaChannel = srcImage.HasAlphaChannel();
			bool srcIsInColorOrder_RGB = srcImage.IsInColorOrder_RGB();
			bool srcIsInColorOrder_BGR = srcImage.IsInColorOrder_BGR();

			// from 24bit/32bit to 8bit CImage, memcpy won't work, have to copy pixel by pixel
			for (int row = 0; row < static_cast<int>(dstImage.GetHeight()); row++) {
				for (int col = 0; col < static_cast<int>(dstImage.GetWidth()); col++) {
					BYTE grayIntensity = 0xFF; // Set Color White if there is Alpha channel
					int srcPixelEntryIndex = row * srcPitch + col * srcChannels;

					if (   ( srcHasAlphaChannel && srcDataEntry[srcPixelEntryIndex + 3] != 0x00 ) // Pure Transparent
						|| !srcHasAlphaChannel )
					{
						// GrayScaledPixel Intensity = 0.2126 * Red + 0.7152 * Green + 0.0722 * Blue;
						if (srcIsInColorOrder_RGB) {
							grayIntensity = static_cast<BYTE>( 0.2126 * srcDataEntry[srcPixelEntryIndex]
															+ 0.7152 * srcDataEntry[srcPixelEntryIndex + 1]
															+ 0.0722 * srcDataEntry[srcPixelEntryIndex + 2] );
						}else if (srcIsInColorOrder_BGR) {
							// CImage will load image with BRG instead of RGB
							grayIntensity = static_cast<BYTE>( 0.2126 * srcDataEntry[srcPixelEntryIndex + 2]
															+ 0.7152 * srcDataEntry[srcPixelEntryIndex + 1]
															+ 0.0722 * srcDataEntry[srcPixelEntryIndex] );
						}else assert(false);
					}

					dstDataEntry[row * dstPitch + col] = grayIntensity;
				}
			}
		}
	}

	/// <summary>Do Image Historgram Equalization, to enhance image contrast</summary>
	/// <remark>Only support LibreColorGray </remark>
	/// <param name="grayScaledImage">Only support SLLibreColorType::LibreColorGray! [IN/OUT]</param>
	void HistorgramEqualization(SLLibreImage& grayScaledImage)
	{
		assert(!grayScaledImage.IsNull() && grayScaledImage.GetColorType() == SLLibreColorType::LibreColorGray);
		BYTE* imageBufferEntry = grayScaledImage.GetBufferEntryForEdit();
		int imageWidth = grayScaledImage.GetWidth();
		int imageHeight = grayScaledImage.GetHeight();
		int imageChannels = grayScaledImage.GetChannels();
		int imageAbsPitch = grayScaledImage.GetPitch();

		int cdf[CPU_TOTAL_GRAY_LEVEL]; // Array of Cumulative Distributive Function at each gray level
		std::memset(cdf, 0, sizeof(cdf));

		// Get Cumulative Distributive Function
		for (int row = 0; row < imageHeight; ++row) {
			for (int column = 0; column < imageWidth; ++column) {
				int pixelGrayLevel = imageBufferEntry[row * imageAbsPitch + column * imageChannels]; // grayLevel == pixel intensity == Red
				cdf[pixelGrayLevel] += 1;
			}
		}
		// find valid min and max of grayLevel
		unsigned int minValidGrayLevel = 0;
		unsigned int maxValidGrayLevel = CPU_TOTAL_GRAY_LEVEL;
		int sumCDF = 0;
		float totalPixel = imageWidth * imageHeight * 1.0f;
		for (int grayLevel = 0; grayLevel < CPU_TOTAL_GRAY_LEVEL; ++grayLevel)
		{
			sumCDF += cdf[grayLevel];
			float lambda = sumCDF / totalPixel;
			if (lambda < VALID_HISTOGRAM_FLOOR_RATIO)           minValidGrayLevel = grayLevel;
			else if (lambda < VALID_HISTOGRAM_CEIL_RATIO)       maxValidGrayLevel = grayLevel;
		}
		// Re-scale imageBufferEntry
		for (int row = 0; row < imageHeight; ++row) {
			for (int column = 0; column < imageWidth; ++column) {
				// ImagePixel on CPU consists of RGBA 4 chanels, each represented by 1 unsigned byte under range 0~255
				int curPixelEntry = row * imageAbsPitch + column * imageChannels;
				BYTE oldIntensity = imageBufferEntry[curPixelEntry];
				BYTE newIntensity = CPU_COLOR_SINGLE_CHANNEL_PURE_BLACK;

				// Calculate New gray-scaled Color Intensity
				if (oldIntensity <= minValidGrayLevel) {
					newIntensity = CPU_COLOR_SINGLE_CHANNEL_PURE_BLACK;
				}else if (oldIntensity > maxValidGrayLevel) {
					newIntensity = CPU_COLOR_SINGLE_CHANNEL_PURE_WHITE;
				}else {
					newIntensity = static_cast<BYTE>( CPU_COLOR_SINGLE_CHANNEL_PURE_WHITE * static_cast<double>(oldIntensity - minValidGrayLevel) / static_cast<double>(maxValidGrayLevel - minValidGrayLevel) );
				}

				// Assign New gray-scaled Color Intensity
				for (int i = 0; i < imageChannels; ++i) {
					imageBufferEntry[curPixelEntry + i] = newIntensity;
				}
			}
		}
	} // End of HistorgramEqualization

	/// <summary>Save SLLibreImage To Disk Image File using CImage </summary>
	/// <remakrs>CImage can process *.bmp, *.gif, *.jpg, *.png, and *.tiff </remakrs>
	/// <param name="imageLoader">Important here!!!  Help Control the Scope of ImageBuffer Life-Time on CPU [OUT]</param>
	/// <param name="filePath">picture filePath + fileName</param>
	void SaveToImageFile(const SLLibreImage& srcImage, const std::wstring& filePath, const SLImageFileType& imageType)
	{
		CImage tmpImageForSaving;

		sldip::DuplicateImage(srcImage, tmpImageForSaving);

		GUID imageTypeGUID = Gdiplus::ImageFormatPNG;
		switch (imageType)
		{
			case SLImageFileType::IMAGE_BMP:            imageTypeGUID = Gdiplus::ImageFormatBMP;            break;
			case SLImageFileType::IMAGE_JPG:            imageTypeGUID = Gdiplus::ImageFormatJPEG;           break;
			case SLImageFileType::IMAGE_PNG:            imageTypeGUID = Gdiplus::ImageFormatPNG;            break;
			default:                                    assert(false);
		}

		HRESULT result = tmpImageForSaving.Save(filePath.c_str(), imageTypeGUID);
		assert(SUCCEEDED(result));
	}
	void SaveToImageFile(const SLLibreImage& srcImage, const std::wstring& folderPath, const std::wstring& fileName, const SLImageFileType& imageType)
	{
		SaveToImageFile(srcImage, (folderPath + fileName), imageType);
	}

	/// <summary>Read picture file from Disk, and return SLImageParam </summary>
	/// <remakrs>SLLibreImage can process *.bmp, *.gif, *.jpg, *.png, and *.tiff </remakrs>
	/// <param name="imageLoader">Important here!!!  Help Control the Scope of ImageBuffer Life-Time on CPU [OUT]</param>
	/// <param name="filePath">picture filePath + fileName</param>
	/// <returns>Image information, except Mapped GPU TextureID  </returns>
	SLImageParam LoadImageParam(SLLibreImage& targetImageLoader, const TCHAR* filePath)
	{
		assert(filePath && _tcsclen(filePath) != 0);

		CImage tmpLoader;
		tmpLoader.Load(filePath);
		assert(!tmpLoader.IsNull()); // failed to load image file or Param

		sldip::DuplicateImage(tmpLoader, targetImageLoader);

		return GetImageParam(targetImageLoader);
	}

	SLImageParam GetImageParam(const SLLibreImage& image)
	{
		assert(!image.IsNull());

		SLImageParam imageParam(
			image.GetWidth(),
			image.GetHeight(),
			image.GetPitch(),
			image.GetColorType(),
			image.GetBufferEntry()
		);

		return imageParam;
	}

} // End of namespace sldip
