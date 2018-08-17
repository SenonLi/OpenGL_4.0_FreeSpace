#ifndef __SLLIBREIMAGE__
#define __SLLIBREIMAGE__
#pragma once

#include "Memory/SLSharedMemoryObject.h"
#include "SLImageParam.h"

namespace sldip
{
	/// <summary>Self image Allocation with help of SLMemoryObject, specially for live scan image GPU-Process</summary>
	class SLLibreImage : public slutil::SLSharedMemoryObject
	{
	public:
		explicit SLLibreImage(unsigned int cols, unsigned int rows, unsigned int channels, int m_Pitch);
		virtual ~SLLibreImage() {};

		unsigned int GetWidth() const { return m_ImageParam.Width(); }
		unsigned int GetHeight() const { return m_ImageParam.Height(); }
		unsigned int GetChannels() const { return m_ImageParam.Channels(); }

	private:
		SLImageParam m_ImageParam{}; // Contains basic image info of SLLibreImage
	};



} // End of namespace sldip

#endif // __SLLIBREIMAGE__
