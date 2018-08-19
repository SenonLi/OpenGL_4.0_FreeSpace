#include "stdafx.h"
#include "SLImageParam.h"

namespace sldip
{
	SLImageParam::SLImageParam(unsigned int cols, unsigned int rows, int pitch, const SLImageColorType& colorType, const BYTE* linearBufferEntry)
		: m_Width(cols), m_Height(rows), m_Pitch(pitch), m_ImageColorType(colorType), m_LinearBufferEntry(linearBufferEntry)
	{
		assert(cols > 0 && rows > 0 && colorType != SLImageColorType::ColorUndefined && linearBufferEntry);
	};
	SLImageParam::SLImageParam(unsigned int cols, unsigned int rows, unsigned int pitch, const SLImageColorType& colorType, const BYTE* linearBufferEntry)
	: SLImageParam(cols, rows, static_cast<int>(pitch), colorType, linearBufferEntry)
	{
	};

	/// <summary> SLImageParam is not supposed to be edited in the air, so that no need to reset </summary>
	void SLImageParam::CreateImageParam(unsigned int cols, unsigned int rows, int pitch, const SLImageColorType& colorType, const BYTE* linearBufferEntry)
	{
		assert(cols > 0 && rows > 0 && colorType != SLImageColorType::ColorUndefined && linearBufferEntry);
		m_Width = cols;
		m_Height = rows;
		m_Pitch = pitch;
		m_ImageColorType = colorType;
		m_LinearBufferEntry = linearBufferEntry;
	}
	void SLImageParam::CreateImageParam(unsigned int cols, unsigned int rows, unsigned int pitch, const SLImageColorType& colorType, const BYTE* linearBufferEntry)
	{
		CreateImageParam(cols, rows, static_cast<int>(pitch), colorType, linearBufferEntry);
	}

	/// <summary>Reset SLImageParam</summary>
	void SLImageParam::Reset()
	{
		m_Width = 0;
		m_Height = 0;
		m_ImageColorType = SLImageColorType::ColorUndefined;
		m_LinearBufferEntry = nullptr;
	}

	unsigned int SLImageParam::GetChannelsNum(const SLImageColorType& colorType)
	{
		int channelsNumber = 0;
		switch (colorType)
		{
			case SLImageColorType::ColorUndefined:		channelsNumber = CHANNEL_NUM_ColorUndefined;	break;
			case SLImageColorType::ColorGray:			channelsNumber = CHANNEL_NUM_ColorGray;			break;
			case SLImageColorType::ColorRGB:			channelsNumber = CHANNEL_NUM_ColorRGB;			break;
			case SLImageColorType::ColorBGR:			channelsNumber = CHANNEL_NUM_ColorBGR;			break;
			case SLImageColorType::ColorRGBA:			channelsNumber = CHANNEL_NUM_ColorRGBA;			break;
			case SLImageColorType::ColorBGRA:			channelsNumber = CHANNEL_NUM_ColorBGRA;			break;
			case SLImageColorType::ColorXYZ:			channelsNumber = CHANNEL_NUM_ColorXYZ;			break;
			case SLImageColorType::ColorXYZW:			channelsNumber = CHANNEL_NUM_ColorXYZW;			break;
			case SLImageColorType::ColorXYZG:			channelsNumber = CHANNEL_NUM_ColorXYZG;			break;
			case SLImageColorType::ColorXYZRGB:			channelsNumber = CHANNEL_NUM_ColorXYZRGB;		break;
			case SLImageColorType::ColorXYZWRGBA:		channelsNumber = CHANNEL_NUM_ColorXYZWRGBA;		break;
			default:									assert(false);
		}
		return channelsNumber;
	}

}// End of namespace sldip