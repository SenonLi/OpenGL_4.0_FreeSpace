#include "../stdafx.h"

#include "SLCImageProcess.h"

#include <vector>

namespace slcimage
{
	//====================================================================================================================
	//--------------------------------------------------------------------------------------------------------------------
	void DuplicateImage(const ATL::CImage& srcImage, ATL::CImage& dstImage)
	{
		assert(!srcImage.IsNull());
		if (srcImage == dstImage)		return;
		if (!dstImage.IsNull())         dstImage.Destroy();

		int srcBitsCount = srcImage.GetBPP();
		if (srcBitsCount == 32)	dstImage.Create(srcImage.GetWidth(), srcImage.GetHeight(), srcBitsCount, CImage::createAlphaChannel);
		else                    dstImage.Create(srcImage.GetWidth(), srcImage.GetHeight(), srcBitsCount, 0);

		if (srcImage.IsIndexed())	{
			// 8-bit: 0x100000000 = 256, in the same way, 4-bit: 0x10000 = 16, 2-bit: 0x100 = 4, 1-bit: 0x10 = 2
			int nColors = srcImage.GetMaxColorTableEntries();
			if (nColors > 0)	{
				std::vector<RGBQUAD> rgbColors(nColors);
				srcImage.GetColorTable(0, nColors, rgbColors.data());
				dstImage.SetColorTable(0, nColors, rgbColors.data());
			}
		}

		srcImage.BitBlt(dstImage.GetDC(), 0, 0, SRCCOPY);
		dstImage.ReleaseDC(); // Remember to dstImage.ReleaseDC() after dstImage.GetDC()
	};// End of DuplicateImage(const CImage& srcImage, CImage& destImage)


	//========================================================================================================================
	//========================================================================================================================
	//------------------------------------------------------------------------------------------------------------------------
	void StretchImageByWidth(const ATL::CImage& srcImage, int widthInPixel, ATL::CImage& dstImage)
	{
		assert(!srcImage.IsNull() && srcImage != dstImage && widthInPixel > 0);

		int srcBitsCount = srcImage.GetBPP();
		double imageAspectRatio = static_cast<double>( srcImage.GetHeight() ) / srcImage.GetWidth() ;
		
		// Get valid dstImageWidth and dstImageHeight
		int dstImageWidth = widthInPixel;
		double tmpDstImageHeight = imageAspectRatio * dstImageWidth;
		int dstImageHeight = tmpDstImageHeight > 1.0 ? static_cast<int>(tmpDstImageHeight) : 1;

		if (!dstImage.IsNull()) dstImage.Destroy();
		if (srcBitsCount == 32)	dstImage.Create(dstImageWidth, dstImageHeight, srcBitsCount, CImage::createAlphaChannel);
		else                    dstImage.Create(dstImageWidth, dstImageHeight, srcBitsCount, 0);

		// For 8bit CImage, we need to copy the ColorTable after image creation
		if (srcImage.IsIndexed()) {
			// 8-bit: 0x100000000 = 256, in the same way, 4-bit: 0x10000 = 16, 2-bit: 0x100 = 4, 1-bit: 0x10 = 2
			int nColors = srcImage.GetMaxColorTableEntries();
			if (nColors > 0) {
				std::vector<RGBQUAD> rgbColors(nColors);
				srcImage.GetColorTable(0, nColors, rgbColors.data());
				dstImage.SetColorTable(0, nColors, rgbColors.data());
			}
		}

		HDC dstHDC = dstImage.GetDC();
		SetStretchBltMode(dstHDC, COLORONCOLOR);
		srcImage.StretchBlt(dstHDC, 0, 0, dstImageWidth, dstImageHeight, SRCCOPY);
		dstImage.ReleaseDC(); // Remember to dstImage.ReleaseDC() after dstImage.GetDC()
	}// End of GetSmallerImageByWidth(const ATL::CImage& srcImage, int widthInPixel, ATL::CImage& dstImage)


	/// <summary>Will help save cost when loading 8bit (GrayScaled) images need to go between CImage and LibreImage </summary>
	void Convert8bitTo24Bit(const ATL::CImage& srcImage, ATL::CImage& dstImage)
	{
		assert(!srcImage.IsNull() && srcImage != dstImage && srcImage.GetBPP() == 8);

		if (!dstImage.IsNull())     dstImage.Destroy();
		dstImage.Create(srcImage.GetWidth(), srcImage.GetHeight(), 24, 0);

		srcImage.BitBlt(dstImage.GetDC(), 0, 0, SRCCOPY);
		dstImage.ReleaseDC(); // Remember to dstImage.ReleaseDC() after dstImage.GetDC()
	}

	void Convert8bitTo32Bit(const ATL::CImage& srcImage, ATL::CImage& dstImage)
	{
		assert(!srcImage.IsNull() && srcImage != dstImage && srcImage.GetBPP() == 8);

		if (!dstImage.IsNull())     dstImage.Destroy();
		dstImage.Create(srcImage.GetWidth(), srcImage.GetHeight(), 32, CImage::createAlphaChannel);
		
		// CImage::BitBlt won't work with alpha channel, we have to set pixel one by one
		for (int row = 0; row < dstImage.GetHeight(); row++)
		{
			for (int col = 0; col < dstImage.GetWidth(); col++)
			{
				BYTE* pixelEntry = static_cast<BYTE*>(dstImage.GetPixelAddress(col, row));
				const BYTE intensity = static_cast<const BYTE*>(srcImage.GetPixelAddress(col, row))[0];
				pixelEntry[0] = intensity;
				pixelEntry[1] = intensity;
				pixelEntry[2] = intensity;
				pixelEntry[3] = 0xFF;
			}
		}
	}


} // End of namespace slcimage
