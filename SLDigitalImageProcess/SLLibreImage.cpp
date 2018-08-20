#include "stdafx.h"
#include "SLLibreImage.h"
#include "StaticConstBasics\SLGeneralImageBasics.h"

namespace sldip
{
	SLLibreImage::SLLibreImage(unsigned int cols, unsigned int rows, const SLLibreColorType& colorType)
	: m_TmpChannels(SLImageParam::GetChannelsNum(colorType))
	, SLSharedMemoryObject(GetAlignedPitch(cols, m_TmpChannels) * rows, GetAlignmentSize(m_TmpChannels))
	, m_ImageParam(cols, rows, static_cast<int>(GetAlignedPitch(cols, m_TmpChannels)), colorType, m_SharedBuffer->m_BufferEntry)
	{
	}

	void SLLibreImage::CreateLibreImage(unsigned int cols, unsigned int rows, const SLLibreColorType& colorType)
	{
		m_TmpChannels = SLImageParam::GetChannelsNum(colorType);
		unsigned int calculatedPitch = GetAlignedPitch(cols, m_TmpChannels);
		// SharedMemoryObject will reset automatically
		CreateSharedMemory(calculatedPitch * rows, GetAlignmentSize(m_TmpChannels));
		// SLImageParam is not supposed to be edited, so that no need to reset
		m_ImageParam.CreateImageParam(cols, rows, static_cast<int>(calculatedPitch), colorType, m_SharedBuffer->m_BufferEntry);
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
		if (channels <= slutil::SL_RGBA_AlignmentSize)              return slutil::SL_RGBA_AlignmentSize;
		else if (channels <= slutil::SL_XYZWRGBA_AlignmentSize)     return slutil::SL_XYZWRGBA_AlignmentSize;
		else {
			assert(false);
			return slutil::SL_XYZWRGBA_AlignmentSize;
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