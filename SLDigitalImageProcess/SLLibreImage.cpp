#include "stdafx.h"
#include "SLLibreImage.h"


namespace sldip
{
	SLLibreImage::SLLibreImage(unsigned int cols, unsigned int rows, unsigned int channels, int pitch)
	: SLSharedMemoryObject(cols * rows * channels)
		, m_ImageParam(cols, rows, pitch, SLImageParam::GetImage2DColorType(channels))
	{
	}


} // End of namespace sldip