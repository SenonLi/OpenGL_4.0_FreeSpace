#include "stdafx.h"

#include "SLCImageHandles.h"
#include "SLImageParam.h"
#include "SLLibreImage.h"
#include "SLDigitalImageProcess.h" // For basic slgip static const definition

#include <vector>

namespace sldip
{
	//====================================================================================================================
	//--------------------------------------------------------------------------------------------------------------------
	void DuplicateImage(const CImage& srcImage, CImage& dstImage)
	{
		assert(!srcImage.IsNull());
		if (srcImage == dstImage)
			return;

		int srcBitsCount = srcImage.GetBPP();
		int srcWidth = srcImage.GetWidth();
		int srcHeight = srcImage.GetHeight();

		if (!dstImage.IsNull())     dstImage.Destroy();
		if (srcBitsCount == 32)     dstImage.Create(srcWidth, srcHeight, srcBitsCount, CImage::createAlphaChannel);
		else                        dstImage.Create(srcWidth, srcHeight, srcBitsCount, 0);

		if (srcImage.IsIndexed())	{
			int nColors = srcImage.GetMaxColorTableEntries();
			if (nColors > 0)	{
				std::vector<RGBQUAD> rgbColors(nColors);
				srcImage.GetColorTable(0, nColors, rgbColors.data());
				dstImage.SetColorTable(0, nColors, rgbColors.data());
			}
		}

		HDC deepCopyHDC = dstImage.GetDC();
		srcImage.BitBlt(deepCopyHDC, 0, 0, SRCCOPY);
		dstImage.ReleaseDC();
	};// End of DuplicateImage(const CImage& srcImage, CImage& destImage)


	//====================================================================================================================
	//--------------------------------------------------------------------------------------------------------------------
	void DuplicateImage(const CImage& srcImage, SLLibreImage& dstImage)
	{
		assert(!srcImage.IsNull());
		dstImage.CreateLibreImage(srcImage.GetWidth(), srcImage.GetHeight(), sldip::GetImage2DColorType(srcImage));
		assert(!dstImage.IsNull());

		int srcPitch = srcImage.GetPitch();
		int dstPitch = dstImage.GetPitch();

		const BYTE* srcBottomLineDataEntry = static_cast<const BYTE*>(srcImage.GetBits());
		BYTE* dstDataEntry = dstImage.GetBufferEntryForEdit();

		for (unsigned int row = 0; row < dstImage.GetHeight(); row++)
		{
			// Attention!! :  Must use int for BYTE* address seeking !
			// For Example:   BYTE* bufferEntry = srcBottomLineDataEntry + static_cast<size_t>(image.GetHeight() - 1) * image.GetPitch() // image.GetPitch() < 0
			memcpy(dstDataEntry + static_cast<int>( row * dstPitch), srcBottomLineDataEntry + static_cast<int>(row * srcPitch), std::abs(srcPitch));
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

		if (srcBitsCount == 32)             dstImage.Create(srcWidth, srcHeight, srcBitsCount, CImage::createAlphaChannel);
		else if (srcBitsCount == 24)        dstImage.Create(srcWidth, srcHeight, srcBitsCount, 0);
		else if (srcBitsCount == 8)         dstImage.Create(srcWidth, srcHeight, 24, 0); // Save to 24bit image, otherwise CImage 8bit need to handle colorTable
		else                                assert(false);

		// CImage belongs to Windows GDI library, in which all DIBs (Device-Independent Bitmap) are bottom-up.
		// GetBits() will return pixel address of (0,0) instead of the first byte of the image-buffer.
		// To get the BufferEntry address for uploading to GPU, here we need to move pointer up [Height() - 1] levels.
		BYTE* dstBottomLineDataEntry = static_cast<BYTE*>(dstImage.GetBits());
		const BYTE* srcDataEntry = srcImage.GetBufferEntry();
		int srcPitch = static_cast<int>(srcImage.GetPitch());

		if (srcBitsCount != 8)		{
			int dstPitch = dstImage.GetPitch();
			for (int row = 0; row < dstImage.GetHeight(); row++)	{
				// Attention!! :  Must use int for BYTE* address seeking !
				// For Example:   BYTE* bufferEntry = srcBottomLineDataEntry + static_cast<size_t>(image.GetHeight() - 1) * image.GetPitch() // image.GetPitch() < 0
				memcpy(dstBottomLineDataEntry + static_cast<int>(row * dstPitch), srcDataEntry + static_cast<int>(row * srcPitch), std::abs(dstPitch));
			}
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
	SLImageColorType GetImage2DColorType(const CImage& image) 
	{
		assert(!image.IsNull());
		SLImageColorType resultColorType = SLImageColorType::ColorUndefined;

		unsigned int channels = static_cast<unsigned int>(image.GetBPP() / sldip::BIT_NUM_IN_ONE_BYTE);
		switch (channels)
		{
			case CHANNEL_NUM_ColorGray:     resultColorType = SLImageColorType::ColorGray;		break;
			case CHANNEL_NUM_ColorBGR:      resultColorType = SLImageColorType::ColorBGR;		break;
			case CHANNEL_NUM_ColorBGRA:     resultColorType = SLImageColorType::ColorBGRA;		break;
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

		// CImage belongs to Windows GDI library, in which all DIBs (Device-Independent Bitmap) are bottom-up.
		// GetBits() will return pixel address of (0,0) instead of the first byte of the image-buffer.
		// To get the BufferEntry address for uploading to GPU, here we need to move pointer up [Height() - 1] levels.
		BYTE* srcBottomLineDataEntry = static_cast<BYTE*>( image.GetBits() );
		// Attention!! :  Must use int for BYTE* address seeking !
		// actualBufferEntry here from CImage is the BottomeLine FirstPixel Address
		BYTE* actualBufferEntry = srcBottomLineDataEntry + static_cast<size_t>(image.GetHeight() - 1) * image.GetPitch();

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
