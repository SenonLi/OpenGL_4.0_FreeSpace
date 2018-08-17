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
		explicit SLLibreImage(unsigned int cols, unsigned int rows, unsigned int channels);
		virtual ~SLLibreImage() {};

		void Reset() override;
		void CreateLibreImage(unsigned int cols, unsigned int rows, unsigned int channels);

		const unsigned int GetWidth() const { return m_ImageParam.Width(); }
		const unsigned int GetHeight() const { return m_ImageParam.Height(); }
		const unsigned int GetChannels() const { return m_ImageParam.Channels(); }
		const unsigned int GetPitch() const { return m_ImageParam.Pitch(); }
		SLImageColorType GetColorType()	const { return m_ImageParam.ColorType(); }

		static unsigned int GetAlignedPitch(unsigned int cols, unsigned int channels);
		static unsigned int GetAlignmentSize(unsigned int channels);
	private:
		/// <remark> SLLibreImage is designed as an Top-down ImageType, and SLImageParam::m_Pitch should not be nagetive </remark>
		///  using Aligned SLSharedMemoryObject with No Padding, SLImageParam::m_Pitch should always equal to cols * channels </remark>
		SLImageParam m_ImageParam{}; // Contains basic image info of SLLibreImage
	};



} // End of namespace sldip

#endif // __SLLIBREIMAGE__
