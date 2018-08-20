#include "stdafx.h"
#include "SLImageParam.h"

namespace sldip
{
	SLImageParam::SLImageParam(unsigned int cols, unsigned int rows, int pitch, const SLLibreColorType& colorType, const BYTE* linearBufferEntry)
		: m_Width(cols), m_Height(rows), m_Pitch(pitch), m_ImageColorType(colorType), m_LinearBufferEntry(linearBufferEntry)
	{
		assert(cols > 0 && rows > 0 && colorType != SLLibreColorType::LibreColorUndefined && linearBufferEntry);
	};
	SLImageParam::SLImageParam(unsigned int cols, unsigned int rows, unsigned int pitch, const SLLibreColorType& colorType, const BYTE* linearBufferEntry)
	: SLImageParam(cols, rows, static_cast<int>(pitch), colorType, linearBufferEntry)
	{
	};

	/// <summary> SLImageParam is not supposed to be edited in the air, so that no need to reset </summary>
	void SLImageParam::CreateImageParam(unsigned int cols, unsigned int rows, int pitch, const SLLibreColorType& colorType, const BYTE* linearBufferEntry)
	{
		assert(cols > 0 && rows > 0 && colorType != SLLibreColorType::LibreColorUndefined && linearBufferEntry);
		m_Width = cols;
		m_Height = rows;
		m_Pitch = pitch;
		m_ImageColorType = colorType;
		m_LinearBufferEntry = linearBufferEntry;
	}
	void SLImageParam::CreateImageParam(unsigned int cols, unsigned int rows, unsigned int pitch, const SLLibreColorType& colorType, const BYTE* linearBufferEntry)
	{
		CreateImageParam(cols, rows, static_cast<int>(pitch), colorType, linearBufferEntry);
	}

	/// <summary>Reset SLImageParam</summary>
	void SLImageParam::Reset()
	{
		m_Width = 0;
		m_Height = 0;
		m_ImageColorType = SLLibreColorType::LibreColorUndefined;
		m_LinearBufferEntry = nullptr;
	}

	unsigned int SLImageParam::GetChannelsNum(const SLLibreColorType& colorType)
	{
		int channelsNumber = 0;
		switch (colorType)
		{
			case SLLibreColorType::LibreColorUndefined:     channelsNumber = CHANNEL_NUM_ColorUndefined;    break;
			case SLLibreColorType::LibreColorGray:          channelsNumber = CHANNEL_NUM_ColorGray;         break;
			case SLLibreColorType::LibreColorRGB:           channelsNumber = CHANNEL_NUM_ColorRGB;          break;
			case SLLibreColorType::LibreColorBGR:           channelsNumber = CHANNEL_NUM_ColorBGR;          break;
			case SLLibreColorType::LibreColorRGBA:          channelsNumber = CHANNEL_NUM_ColorRGBA;         break;
			case SLLibreColorType::LibreColorBGRA:          channelsNumber = CHANNEL_NUM_ColorBGRA;         break;
			case SLLibreColorType::LibreColorXYZ:           channelsNumber = CHANNEL_NUM_ColorXYZ;          break;
			case SLLibreColorType::LibreColorXYZW:          channelsNumber = CHANNEL_NUM_ColorXYZW;         break;
			case SLLibreColorType::LibreColorXYZG:          channelsNumber = CHANNEL_NUM_ColorXYZG;         break;
			case SLLibreColorType::LibreColorXYZRGB:        channelsNumber = CHANNEL_NUM_ColorXYZRGB;       break;
			case SLLibreColorType::LibreColorXYZWRGBA:      channelsNumber = CHANNEL_NUM_ColorXYZWRGBA;     break;
			default:                                        assert(false);
		}
		return channelsNumber;
	}

}// End of namespace sldip