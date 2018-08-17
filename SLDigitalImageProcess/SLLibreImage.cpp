#include "stdafx.h"
#include "SLLibreImage.h"


namespace sldip
{
	/// <summary>    Image data that you pass to OpenGL is expected to be grouped into rows.
	///       Each row contains width number of pixels, with each pixel being the size as defined by the format and type parameters.
	///       So a format of GL_RGB with a type of GL_UNSIGNED_BYTE will result in a pixel that is 24-bits in size. 
	///       Pixels are otherwise expected to be packed, so a row of 16 pixels (GL_RGB) will take up 48 bytes.
	///              Each row is expected to be aligned on a specific value, as defined by the GL_PACK / UNPACK_ALIGNMENT.
	///       This means that the value you add to the pointer to get to the next row is : align(pixel_size * width, GL_*_ALIGNMENT).
	///       If the pixel (RGB) size is 3 bytes, the width is 2, and the alignment is 1 (Byte), the row byte size is 6.
	///       If the alignment is 4 (Bytes), the row byte size is eight.  </summary>
	/// <remark>The alignment: 4 Bytes is designed for RGBA; 16 for XYXWRGBA</remark>
	static const unsigned int SL_RGBA_AlignmentSize = 4; // must be an integer power of 2
	static const unsigned int SL_XYZWRGBA_AlignmentSize = 16; // must be an integer power of 2

	SLLibreImage::SLLibreImage(unsigned int cols, unsigned int rows, unsigned int channels)
	: SLSharedMemoryObject(GetAlignedPitch(cols, channels) * rows, GetAlignmentSize(channels))
	, m_ImageParam(cols, rows, static_cast<int>(GetAlignedPitch(cols, channels)), SLImageParam::GetImage2DColorType(channels), m_SharedBuffer->m_BufferEntry)
	{
	}

	void SLLibreImage::CreateLibreImage(unsigned int cols, unsigned int rows, unsigned int channels)
	{
		unsigned int calculatedPitch = GetAlignedPitch(cols, channels);
		// SharedMemoryObject will reset automatically
		CreateSharedMemory(calculatedPitch * rows, GetAlignmentSize(channels));
		// SLImageParam is not supposed to be edited, so that no need to reset
		m_ImageParam.CreateImageParam(cols, rows, static_cast<int>(calculatedPitch), SLImageParam::GetImage2DColorType(channels), m_SharedBuffer->m_BufferEntry);
	}

	unsigned int SLLibreImage::GetAlignedPitch(unsigned int cols, unsigned int channels)
	{
		unsigned int alignmentSize = SLLibreImage::GetAlignmentSize(channels);
		unsigned int validRowPixelBytes = cols * channels;
		if (validRowPixelBytes % alignmentSize == 0)
			return validRowPixelBytes;
		else {
			return (validRowPixelBytes / alignmentSize) * alignmentSize + alignmentSize;
		}
	}
	unsigned int SLLibreImage::GetAlignmentSize(unsigned int channels)
	{
		if (channels <= SL_RGBA_AlignmentSize)              return SL_RGBA_AlignmentSize;
		else if (channels <= SL_XYZWRGBA_AlignmentSize)     return SL_XYZWRGBA_AlignmentSize;
		else {
			assert(false);
			return SL_XYZWRGBA_AlignmentSize;
		}
	}

	/// <summary>Reset SLLibreImage</summary>
	void SLLibreImage::Reset()
	{
		if (!IsNull())
		{
			SLSharedMemoryObject::Reset();
			m_ImageParam.Reset();
		}
	}
} // End of namespace sldip