#ifndef __SLGENERALIMAGEBASICS__
#define __SLGENERALIMAGEBASICS__
#pragma once


namespace slutil
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
	static const unsigned int SL_MIN_STORAGE_PACKING_AlignmentSize = 1;

	static const int           BYTE_IMAGE_SINGLE_CHANNEL_BITS = 8;
	static const int           GRAYSCALED_IMAGE_BIT_PER_PIXEL = 8;
	static const int           MAX_CPU_SINGLE_CHANNEL_VALUE_INT = 255;
	static const unsigned int  MAX_CPU_SINGLE_CHANNEL_VALUE_UNSIGNED_INT = 255;

	//========================================================================
	//-------------        Edge Detection       ------------------------------
	static const int   MAX_CANNY_THRESH_RATIO_INT = 35;



}; // End of namespace slutil




#endif // __SLGENERALIMAGEBASICS__