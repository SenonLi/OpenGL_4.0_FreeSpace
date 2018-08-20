#ifndef __SLLIBREIMAGE__
#define __SLLIBREIMAGE__
#pragma once

#include "Memory/SLSharedMemoryObject.h"
#include "SLImageParam.h"

namespace sldip
{
	/// <summary>Aligned Allocation Image with No Padding, using SLSharedMemoryObject, specially for live image GPU-Process </summary>
	/// <remark> using Aligned SLSharedMemoryObject with No Padding, SLImageParam::m_Pitch should always equal to cols * channels </remark>
	/// <remark> The alignment: 4 Bytes is designed for RGBA for now; 16 for XYXWRGBA</remark>
	class SLLibreImage : public slutil::SLSharedMemoryObject
	{
	public:
		SLLibreImage() {};
		explicit SLLibreImage(unsigned int cols, unsigned int rows, const SLLibreColorType& colorType);
		virtual ~SLLibreImage() {};

		static unsigned int GetAlignedPitch(unsigned int cols, unsigned int channels);
		static unsigned int GetAlignmentSize(unsigned int channels);

		void Reset() override;
		void CreateLibreImage(unsigned int cols, unsigned int rows, const SLLibreColorType& colorType);

		const unsigned int      GetWidth()          const { return m_ImageParam.Width(); }
		const unsigned int      GetHeight()	        const { return m_ImageParam.Height(); }
		const unsigned int      GetChannels()		const { return m_ImageParam.Channels(); }
		const unsigned int      GetPitch()          const { return m_ImageParam.Pitch(); }
		const SLLibreColorType	GetColorType()	    const { return m_ImageParam.ColorType(); }
		const bool              HasAlphaChannel()   const { return m_ImageParam.HasAlphaChannel(); }
		const bool              IsInColorOrder_RGB()const { return m_ImageParam.IsInColorOrder_RGB(); }
		const bool              IsInColorOrder_BGR()const { return m_ImageParam.IsInColorOrder_BGR(); }

	private:
		/// <remark> SLLibreImage is designed as an Top-down ImageType, and SLImageParam::m_Pitch should not be nagetive </remark>
		///  using Aligned SLSharedMemoryObject with No Padding, SLImageParam::m_Pitch should always equal to cols * channels </remark>
		SLImageParam m_ImageParam{}; // Contains basic image info of SLLibreImage

		/// <summary>m_TmpChannels is only a helper for Constructor
		/// 1. Calculate m_TmpChannels from SLLibreColorType, 
		/// 2. then use channels to allocate m_SharedBuffer->m_BufferEntry 
		/// 3. then use m_SharedBuffer->m_BufferEntry along with m_TmpChannels to constructor m_ImageParam </summary>
		unsigned int m_TmpChannels = 0;
	};



} // End of namespace sldip

#endif // __SLLIBREIMAGE__
