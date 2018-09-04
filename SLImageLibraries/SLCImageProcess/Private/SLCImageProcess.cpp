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

	void Convert24bitTo32Bit(const ATL::CImage& srcImage, ATL::CImage& dstImage)
	{
		assert(!srcImage.IsNull() && srcImage != dstImage && srcImage.GetBPP() == 24);

		if (!dstImage.IsNull())     dstImage.Destroy();
		dstImage.Create(srcImage.GetWidth(), srcImage.GetHeight(), 32, CImage::createAlphaChannel);

		for (int row = 0; row < dstImage.GetHeight(); row++)
		{
			for (int col = 0; col < dstImage.GetWidth(); col++)
			{
				BYTE* dstPixelEntry = static_cast<BYTE*>(dstImage.GetPixelAddress(col, row));
				const BYTE* srcPixelEntry = static_cast<const BYTE*>(srcImage.GetPixelAddress(col, row));
				dstPixelEntry[0] = srcPixelEntry[0];
				dstPixelEntry[1] = srcPixelEntry[1];
				dstPixelEntry[2] = srcPixelEntry[2];
				dstPixelEntry[3] = 0xFF;
			}
		}
	}

} // End of namespace slcimage
