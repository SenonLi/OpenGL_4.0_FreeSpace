#include "stdafx.h"

#include "SLLibreImageCImageHandles.h"
#include "SLImageParam.h"
#include "SLLibreImage.h"
#include "SLDigitalImageProcess.h" // For basic sldip static const definition
#include "StaticConstBasics\SLGeneralImageBasics.h"

#include <vector>

namespace sldip
{
	static void DuplicateLibreSupportedImage(const CImage& srcImage, SLLibreImage& dstImage)
	{
		assert(!srcImage.IsNull() && srcImage.GetBPP() >= 8);
		dstImage.CreateLibreImage(srcImage.GetWidth(), srcImage.GetHeight(), sldip::GetImage2DColorType(srcImage));
		assert(!dstImage.IsNull());

		int srcPitch = srcImage.GetPitch();
		int dstPitch = dstImage.GetPitch();
		assert(std::abs(srcPitch) == std::abs(dstPitch));

		const BYTE* srcFirstPixelDataEntry = static_cast<const BYTE*>(srcImage.GetBits());
		BYTE* dstDataEntry = dstImage.GetBufferEntryForEdit();

		// Pitch > 0, image is Top-Down; Pitch < 0, image is Bottom-Up.
		if(srcPitch > 0)
			memcpy(dstDataEntry, srcFirstPixelDataEntry, srcPitch * srcImage.GetHeight() );
		else
		{
			for (unsigned int row = 0; row < dstImage.GetHeight(); row++)
			{
				// Attention!! :  Must use int for BYTE* address seeking !
				// For Example:   BYTE* bufferEntry = srcFirstPixelDataEntry + static_cast<size_t>(image.GetHeight() - 1) * image.GetPitch() // image.GetPitch() < 0
				memcpy(dstDataEntry + static_cast<int>(row * dstPitch), srcFirstPixelDataEntry + static_cast<int>(row * srcPitch), std::abs(dstPitch));
			}
		}
	}

	//====================================================================================================================
	//--------------------------------------------------------------------------------------------------------------------
	void DuplicateImage(const CImage& srcImage, SLLibreImage& dstImage)
	{
		assert(!srcImage.IsNull());
		if (srcImage.GetBPP() >= slutil::GRAYSCALED_IMAGE_BIT_PER_PIXEL)
			DuplicateLibreSupportedImage(srcImage, dstImage);
		else {
			// LibreImage does not support BPP smaller than 8, we Need to Get 8bit CImage first, then duplicate to LibreImage
			CImage tmp8bit;
			slcimage::Convert8BitBelowToAbove(srcImage, 8, tmp8bit);
			DuplicateLibreSupportedImage(tmp8bit, dstImage);
		}
	}// End of DuplicateImage(const CImage& srcImage, SLLibreImage& dstImage)


	//====================================================================================================================
	//--------------------------------------------------------------------------------------------------------------------
	void DuplicateImage(const SLLibreImage& srcImage, CImage& dstImage)
	{
		assert(!srcImage.IsNull());
		if (!dstImage.IsNull())     dstImage.Destroy();

		int srcBitsCount = srcImage.GetChannels() * sldip::BIT_NUM_IN_ONE_BYTE;
		int srcWidth = srcImage.GetWidth();
		int srcHeight = srcImage.GetHeight();

		// CImage belongs to Windows GDI library, in which all default DIBs (Device-Independent Bitmap) are bottom-up;
		// i.e., the default Pitch < 0, unless you create CImage with input targetHeight < 0,
		// in which case, the generated CImage would have hight still > 0, but Pitch would be > 0.
		// GetBits() will return pixel address of (0,0) instead of the Top-Line byte of the image-buffer.
		// To get the BufferEntry address for uploading to GPU, 
		// if Pitch > 0, GetBits() will work; 
		// if Pitch < 0 (by default), here we need to move pointer up [Height() - 1] levels.

		// Here we create CImage with negative Input Height for fast data copy, 
		// otherwise, for 32bit and 24bit, we need to do copy line by line.
		if (srcBitsCount == 32)             dstImage.Create(srcWidth, -srcHeight, srcBitsCount, CImage::createAlphaChannel);
		else if (srcBitsCount == 24)        dstImage.Create(srcWidth, -srcHeight, srcBitsCount, 0);
		else if (srcBitsCount == 8)         dstImage.Create(srcWidth, -srcHeight, 24, 0); // Save to 24bit image, otherwise CImage 8bit need to handle colorTable
		else                                assert(false);

		BYTE* dstFirstPixelDataEntry = static_cast<BYTE*>(dstImage.GetBits());
		const BYTE* srcDataEntry = srcImage.GetBufferEntry();
		int srcPitch = static_cast<int>(srcImage.GetPitch());

		if (srcBitsCount != 8)		{
			int dstPitch = dstImage.GetPitch();
			memcpy(dstFirstPixelDataEntry, srcDataEntry, dstPitch * srcHeight);
		}else {
			// from 8bit to 24bit CImage, memcpy won't work, have to copy pixel by pixel
			for (int row = 0; row < dstImage.GetHeight(); row++) {
				for (int col = 0; col < dstImage.GetWidth(); col++) {
					BYTE intensity = srcDataEntry[static_cast<int>(row * srcPitch) + col];
					dstImage.SetPixel(col, row, RGB(intensity, intensity, intensity));
				}
			}
		}

		assert(!dstImage.IsNull());
	};// End of DuplicateImage(const SLLibreImage& srcImage, CImage& dstImage)

	/// <remark>CImage save a pixel in color order BGR instead of RGB </remark>
	SLLibreColorType GetImage2DColorType(const CImage& image) 
	{
		assert(!image.IsNull());
		SLLibreColorType resultColorType = SLLibreColorType::LibreColorUndefined;

		unsigned int channels = CHANNEL_NUM_ColorUndefined;
		if (image.GetBPP() < 8)
			channels = CHANNEL_NUM_ColorGray;
		else
			channels = static_cast<unsigned int>(image.GetBPP() / sldip::BIT_NUM_IN_ONE_BYTE);
		switch (channels)
		{
			case CHANNEL_NUM_ColorGray:     resultColorType = SLLibreColorType::LibreColorGray;		break;
			case CHANNEL_NUM_ColorBGR:      resultColorType = SLLibreColorType::LibreColorBGR;		break;
			case CHANNEL_NUM_ColorBGRA:     resultColorType = SLLibreColorType::LibreColorBGRA;		break;
			default:                        assert(false);
		}
		
		return resultColorType;
	}


	/// <summary>Extract basic ImageParam from Full-Loaded CImage</summary>
	/// <param name="image">Full-Loaded CImage, whose buffer cannot be empty [IN]</param>
	/// <returns>basic ImageParam of the input image, whose LinearBufferEntry cannot be nullptr </returns>
	SLImageParam GetImageParam(CImage& image)
	{
		assert(!image.IsNull());

		// CImage belongs to Windows GDI library, in which all default DIBs (Device-Independent Bitmap) are bottom-up;
		// i.e., the default Pitch < 0, unless you create CImage with input targetHeight < 0,
		// in which case, the generated CImage would have hight still > 0, but Pitch would be > 0.
		// GetBits() will return pixel address of (0,0) instead of the Top-Line byte of the image-buffer.
		// To get the BufferEntry address for uploading to GPU, 
		// if Pitch > 0, GetBits() will work; 
		// if Pitch < 0 (by default), here we need to move pointer up [Height() - 1] levels.
		BYTE* actualBufferEntry = nullptr;
		if (image.GetPitch() > 0)
			actualBufferEntry = static_cast<BYTE*>(image.GetBits());
		else {
			// Attention!! :  Must use int for BYTE* address seeking !
			// actualBufferEntry here from CImage is the BottomeLine FirstPixel Address
			actualBufferEntry = static_cast<BYTE*>(image.GetBits()) + static_cast<size_t>(image.GetHeight() - 1) * image.GetPitch();
		}

		SLImageParam imageParam(
			static_cast<unsigned int>(image.GetWidth()),
			static_cast<unsigned int>(image.GetHeight()),
			image.GetPitch(),
			sldip::GetImage2DColorType(image),
			actualBufferEntry
		);

		return imageParam;
	}// End of GetImageParam(CImage& image)


	/// <summary>Read picture file from Disk, and return SLImageParam </summary>
	/// <remakrs>CImage can process *.bmp, *.gif, *.jpg, *.png, and *.tiff </remakrs>
	/// <param name="imageLoader">Important here!!!  Help Control the Scope of ImageBuffer Life-Time on CPU [OUT]</param>
	/// <param name="filePath">picture filePath + fileName</param>
	/// <returns>Image information, except Mapped GPU TextureID  </returns>
	SLImageParam LoadImageParam(CImage& imageLoader, const TCHAR* filePath)
	{
		assert(filePath && _tcsclen(filePath) != 0);
		if (!imageLoader.IsNull())
			imageLoader.Destroy();
		imageLoader.Load(filePath);
		assert(!imageLoader.IsNull()); // failed to load image file or Param

		return GetImageParam(imageLoader);
	}



} // End of namespace sldip
