#include "stdafx.h"
#include "SLImageParam.h"

namespace sldip
{
	SLImageParam::SLImageParam(unsigned int cols, unsigned int rows, int pitch, const SLImageColorType& colorType)
		: m_Width(cols), m_Height(rows), m_Pitch(pitch), m_ImageColorType(colorType)
	{};

	unsigned int SLImageParam::GetChannelsNum(const SLImageColorType& colorType)
	{
		int channelsNumber = 0;
		switch (colorType)
		{
		case SLImageColorType::ColorUndefined:
			channelsNumber = CHANNEL_NUM_ColorUndefined;
			break;
		case SLImageColorType::ColorGray:
			channelsNumber = CHANNEL_NUM_ColorGray;
			break;
		case SLImageColorType::ColorRGB:
			channelsNumber = CHANNEL_NUM_ColorRGB;
			break;
		case SLImageColorType::ColorRGBA:
			channelsNumber = CHANNEL_NUM_ColorRGBA;
			break;
		case SLImageColorType::ColorXYZ:
			channelsNumber = CHANNEL_NUM_ColorXYZ;
			break;
		case SLImageColorType::ColorXYZW:
			channelsNumber = CHANNEL_NUM_ColorXYZW;
			break;
		case SLImageColorType::ColorXYZG:
			channelsNumber = CHANNEL_NUM_ColorXYZG;
			break;
		case SLImageColorType::ColorXYZRGB:
			channelsNumber = CHANNEL_NUM_ColorXYZRGB;
			break;
		case SLImageColorType::ColorXYZWRGBA:
			channelsNumber = CHANNEL_NUM_ColorXYZWRGBA;
			break;
		}
		return channelsNumber;
	}

	SLImageColorType SLImageParam::GetImage2DColorType(unsigned int image2DChannelNumber)
	{
		SLImageColorType colorType = SLImageColorType::ColorUndefined;
		switch (image2DChannelNumber)
		{
		case CHANNEL_NUM_ColorGray:
			colorType = SLImageColorType::ColorGray;
			break;
		case CHANNEL_NUM_ColorRGB:
			colorType = SLImageColorType::ColorRGB;
			break;
		case CHANNEL_NUM_ColorRGBA:
			colorType = SLImageColorType::ColorRGBA;
			break;
		default:
			assert(false);
		}

		assert(colorType != SLImageColorType::ColorUndefined);
		return colorType;
	}

	/// <summary>Will be used to determine 2D ImageColorType when loading image files from disk </summary>
	/// <param name="image2DChannelNumber">Range could only be 1, 3, 4 </param>
	void SLImageParam::SetImageColorType(unsigned int image2DChannelNumber)
	{
		SetImageColorType(SLImageParam::GetImage2DColorType(image2DChannelNumber));
	}

}// End of namespace sldip