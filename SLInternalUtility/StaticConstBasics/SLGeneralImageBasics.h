#ifndef __SLGENERALIMAGEBASICS__
#define __SLGENERALIMAGEBASICS__
#pragma once


namespace slutil
{
	// Both of the Alignment below are GPU alignment !
	static const unsigned int SL_DEFAULT_STORAGE_PACKING_AlignmentSize = 4;
	static const unsigned int SL_MIN_STORAGE_PACKING_AlignmentSize = 1;

	/// <remark>According to Wikipedia, the bitmap file format specifies that:
	/// The bits representing the bitmap pixels are packed in rows. The size of each row is rounded up to a multiple of 4 bytes(a 32 - bit DWORD) by padding.
	/// Padding bytes(not necessarily 0) must be appended to the end of the rows in order to bring up the length of the rows to a multiple of four bytes.
	/// When the pixel array is loaded into memory, each row must begin at a memory address that is a multiple of 4. 
	/// This address / offset restriction is mandatory only for Pixel Arrays loaded in memory.
	/// For file storage purposes, only the size of each row must be a multiple of 4 bytes while the file offset can be arbitrary.
	/// A 24 - bit bitmap with Width = 1, would have 3 bytes of data per row(blue, green, red) and 1 byte of padding, while Width = 2 would have 2 bytes of padding,
	/// Width = 3 would have 3 bytes of padding, and Width = 4 would not have any padding at all. </remark>
	static const unsigned int SL_BitmapLeastRowBlockSize = 4;

	static const int           BYTE_IMAGE_SINGLE_CHANNEL_BITS = 8;
	static const int           GRAYSCALED_IMAGE_BIT_PER_PIXEL = 8;
	static const int           MAX_CPU_SINGLE_CHANNEL_VALUE_INT = 255;
	static const unsigned int  MAX_CPU_SINGLE_CHANNEL_VALUE_UNSIGNED_INT = 255;

	//========================================================================
	//-------------        Edge Detection       ------------------------------
	static const int   MAX_CANNY_THRESH_RATIO_INT = 35;



}; // End of namespace slutil




#endif // __SLGENERALIMAGEBASICS__