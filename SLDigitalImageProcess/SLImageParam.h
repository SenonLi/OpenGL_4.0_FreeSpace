#ifndef __SLIMAGEOBJECT__
#define __SLIMAGEOBJECT__
#pragma once

namespace sldip
{
	static const unsigned int CHANNEL_NUM_ColorUndefined = 0;
	static const unsigned int CHANNEL_NUM_ColorGray      = 1;
	static const unsigned int CHANNEL_NUM_ColorRGB       = 3;
	static const unsigned int CHANNEL_NUM_ColorRGBA      = 4;
	static const unsigned int CHANNEL_NUM_ColorXYZ       = 3;
	static const unsigned int CHANNEL_NUM_ColorXYZW      = 4;
	static const unsigned int CHANNEL_NUM_ColorXYZG      = 4;
	static const unsigned int CHANNEL_NUM_ColorXYZRGB    = 6;
	static const unsigned int CHANNEL_NUM_ColorXYZWRGBA  = 8;
	
	enum SLImageFileType
	{
		IMAGE_BMP,          // Gdiplus::ImageFormatBMP
		IMAGE_JPG,          // Gdiplus::ImageFormatJPEG
		IMAGE_PNG,          // Gdiplus::ImageFormatPNG
	};

	enum SLImageColorType
	{
		ColorUndefined,	    // Invalid
		ColorGray,          // 2D,      8  BBP,     1 color channel
		ColorRGB,           // 2D,      24 BBP,     3 color channel
		ColorRGBA,          // 2D,      32 BBP,     4 color channel
		ColorXYZ,           // 3D,      24 BBP,     3 Coord channel
		ColorXYZW,          // 3D,      32 BBP,     4 Coord channel, W is part of homogeneous coordinate, for Normalization
		ColorXYZG,          // 3D,      32 BBP,     3 Coord channel and 1 color (Gray/Depth) channel
		ColorXYZRGB,        // 3D,      48 BBP,     3 color channel and 3 Coord channel
		ColorXYZWRGBA,      // 3D,      64 BBP,     4 Coord channel and 4 Coord channel
	};

	class SLImageParam
	{
	public:
		SLImageParam() {};
		SLImageParam(unsigned int cols, unsigned int rows, int pitch, const SLImageColorType& colorType);
		static unsigned int GetChannelsNum(const SLImageColorType& colorType);
		static SLImageColorType GetImage2DColorType(unsigned int channelNumber);

		inline unsigned int Width()      const { return m_Width; }
		inline unsigned int Height()     const { return m_Height; }
		inline int Pitch()      const { return m_Pitch; }
		inline int PitchAbsolute()      const { return std::abs(m_Pitch); }
		inline int Channels() const { return GetChannelsNum( ColorType() ); }
		inline GLuint TextureID() const { return m_TextureID; }
		inline SLImageColorType ColorType()	const { return m_ImageColorType; }
		inline BYTE* LinearBufferEntry() const { return m_LinearBufferEntry; }
		inline bool IsImageBufferNull() const { return (m_LinearBufferEntry == nullptr); }

		inline void SetWidth(unsigned int width) { m_Width = width; }
		inline void SetHeight(unsigned int height) { m_Height = height; }
		inline void SetPitch(int pitch) { m_Pitch = pitch; }
		inline void SetTextureID(GLuint textureID) { m_TextureID = textureID; }
		inline void SetImageColorType(SLImageColorType channels) { m_ImageColorType = channels; }
		inline void SetLinearBufferEntry(BYTE* entry) { m_LinearBufferEntry = entry; }

		void SetImageColorType(unsigned int channelNumber);

	private:
		GLuint m_TextureID = 0; // 0 means Binding-Free for OpenGL driver, i.e., no texture binding for GPU 

		unsigned int m_Width  = 0;
		unsigned int m_Height = 0;
		/// <summary>Or Stride, Length (in Bytes) of an image row in memory: Pitch =  NumRowPixels * BytesPerPixel + Padding </summary>
		/// <remarks>In GDI library, where all DIBs (CImage) are bottom-up, m_Pitch may always be negative <remarks>
		int m_Pitch  = 0; // Negative in CImage, cannot be unsigned int
		SLImageColorType m_ImageColorType = SLImageColorType::ColorUndefined;

		// When image is loaded into CImage (linear CPU memory), m_LinearBufferEntry is beginning Byte address.
		// Just an EntryAddress, not responsible for allocate/deallocate
		BYTE* m_LinearBufferEntry = nullptr;
	};




} // End of namespace sldip


#endif // __SLIMAGEOBJECT__