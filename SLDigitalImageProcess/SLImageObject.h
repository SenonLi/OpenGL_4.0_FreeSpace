#ifndef __SLIMAGEOBJECT__
#define __SLIMAGEOBJECT__
#pragma once

#include "Memory/SLMemoryObject.h"
namespace sldip
{
	static const int CHANNEL_NUM_ColorUndefined = 0;
	static const int CHANNEL_NUM_ColorGray      = 1;
	static const int CHANNEL_NUM_ColorRGB       = 3;
	static const int CHANNEL_NUM_ColorRGBA      = 4;
	static const int CHANNEL_NUM_ColorXYZ       = 3;
	static const int CHANNEL_NUM_ColorXYZW      = 4;
	static const int CHANNEL_NUM_ColorXYZG      = 4;
	static const int CHANNEL_NUM_ColorXYZRGB    = 6;
	static const int CHANNEL_NUM_ColorXYZWRGBA  = 8;

	enum SLImageColorType
	{
		ColorUndefined,	    // Invalid
		ColorGray,          // 2D,      8  BBP,     1 color channel
		ColorRGB,           // 2D,      24 BBP,     3 color channel
		ColorRGBA,          // 2D,      32 BBP,     4 color channel
		ColorXYZ,           // 3D,      24 BBP,     3 Coord channel
		ColorXYZW,          // 3D,      32 BBP,     4 Coord channel, W is part of homogeneous coordinate, for Normalization
		ColorXYZG,          // 3D,      32 BBP,     3 Coord channel and 1 color (Gray) channel
		ColorXYZRGB,        // 3D,      48 BBP,     3 color channel and 3 Coord channel
		ColorXYZWRGBA,      // 3D,      64 BBP,     4 Coord channel and 4 Coord channel
	};

	class SLImageObject : public slutil::SLMemoryObject
	{
	public:
		static int GetChannelsNum(const SLImageColorType& colorType);

		inline int Width()      const { return m_Width; }
		inline int Height()     const { return m_Height; }
		inline int Pitch()      const { return m_Pitch; }
		inline int ChannelNumber() const { return GetChannelsNum( ColorType() ); }
		inline SLImageColorType ColorType()	const { return m_ImageColorType; }

		inline void SetWidth(int width) { m_Width = width; }
		inline void SetHeight(int height) { m_Height = height; }
		inline void SetPitch(int pitch) { m_Pitch = pitch; }
		inline void SetImageColorType(SLImageColorType channels) { m_ImageColorType = channels; }

		void SetImageColorType(int channelNumber);

	private:
		int m_Width  = 0;
		int m_Height = 0;
		int m_Pitch  = 0;
		SLImageColorType m_ImageColorType = SLImageColorType::ColorUndefined;
	};

} // End of namespace sldip


#endif // __SLIMAGEOBJECT__