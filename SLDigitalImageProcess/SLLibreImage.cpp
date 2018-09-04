#include "stdafx.h"
#include "SLLibreImage.h"
#include "StaticConstBasics\SLGeneralImageBasics.h"

namespace sldip
{
	SLLibreImage::SLLibreImage(unsigned int cols, unsigned int rows, const SLLibreColorType& colorType)
	: m_TmpChannels(SLImageParam::GetChannelsNum(colorType))
	, SLSharedMemoryObject(GetPitchWithValidPadding(cols, m_TmpChannels) * rows)
	, m_ImageParam(cols, rows, static_cast<int>(GetPitchWithValidPadding(cols, m_TmpChannels)), colorType, m_SharedBuffer->m_BufferEntry)
	{
	}

	void SLLibreImage::CreateLibreImage(unsigned int cols, unsigned int rows, const SLLibreColorType& colorType)
	{
		m_TmpChannels = SLImageParam::GetChannelsNum(colorType);
		unsigned int calculatedPitch = GetPitchWithValidPadding(cols, m_TmpChannels);
		// SharedMemoryObject will reset automatically
		CreateSharedMemory(calculatedPitch * rows);
		// SLImageParam is not supposed to be edited, so that no need to reset
		m_ImageParam.CreateImageParam(cols, rows, static_cast<int>(calculatedPitch), colorType, m_SharedBuffer->m_BufferEntry);
	}

	unsigned int SLLibreImage::GetPitchWithValidPadding(unsigned int cols, unsigned int channels)
	{
		// See remark of slutil::SL_BitmapLeastRowBlockSize for how padding is added
		unsigned int validRowPixelBytes = cols * channels;
		if (validRowPixelBytes % slutil::SL_BitmapLeastRowBlockSize == 0)
			return validRowPixelBytes;
		else {
			return (validRowPixelBytes / slutil::SL_BitmapLeastRowBlockSize) * slutil::SL_BitmapLeastRowBlockSize + slutil::SL_BitmapLeastRowBlockSize;
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