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

} // End of namespace slcimage
