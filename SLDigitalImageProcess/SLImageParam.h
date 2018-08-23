#ifndef __SLIMAGEOBJECT__
#define __SLIMAGEOBJECT__
#pragma once

namespace sldip
{
	static const unsigned int CHANNEL_NUM_ColorUndefined = 0;
	static const unsigned int CHANNEL_NUM_ColorGray      = 1;
	static const unsigned int CHANNEL_NUM_ColorRGB       = 3;
	static const unsigned int CHANNEL_NUM_ColorBGR       = 3;
	static const unsigned int CHANNEL_NUM_ColorRGBA      = 4;
	static const unsigned int CHANNEL_NUM_ColorBGRA      = 4;
	static const unsigned int CHANNEL_NUM_ColorXYZ       = 3;
	static const unsigned int CHANNEL_NUM_ColorXYZW      = 4;
	static const unsigned int CHANNEL_NUM_ColorXYZG      = 4;
	static const unsigned int CHANNEL_NUM_ColorXYZRGB    = 6;
	static const unsigned int CHANNEL_NUM_ColorXYZWRGBA  = 8;
	
	enum SLImageFileType
	{
		IMAGE_BMP,          // Gdiplus::ImageFormatBMP
		IMAGE_JPG,          // Gdiplus::ImageFormatJPEG
		IMAGE_PNG           // Gdiplus::ImageFormatPNG
	};

	enum SLLibreColorType
	{
		LibreColorUndefined,	 // Invalid
		LibreColorGray,          // 2D,      8  BBP,     1 color channel
		LibreColorRGB,           // 2D,      24 BBP,     3 color channel
		LibreColorBGR,           // 2D,      24 BBP,     3 color channel,   CImage
		LibreColorRGBA,          // 2D,      32 BBP,     4 color channel
		LibreColorBGRA,          // 2D,      32 BBP,     4 color channel,   CImage
		LibreColorXYZ,           // 3D,      24 BBP,     3 Coord channel
		LibreColorXYZW,          // 3D,      32 BBP,     4 Coord channel, W is part of homogeneous coordinate, for Normalization
		LibreColorXYZG,          // 3D,      32 BBP,     3 Coord channel and 1 color (Gray/Depth) channel
		LibreColorXYZRGB,        // 3D,      48 BBP,     3 color channel and 3 Coord channel
		LibreColorXYZWRGBA       // 3D,      64 BBP,     4 Coord channel and 4 Coord channel
	};

	/// <summary>SLImageParam is designed for ImageParam passing
	///           it is designed to be an envelope for delivery, and one should not try to edit it after well initialed .</summary>
	class SLImageParam
	{
	public:
		SLImageParam() = default; // Default constructor here for class member initialization
		SLImageParam(unsigned int cols, unsigned int rows, int pitch, const SLLibreColorType& colorType, const BYTE* linearBufferEntry);
		SLImageParam(unsigned int cols, unsigned int rows, unsigned int pitch, const SLLibreColorType& colorType, const BYTE* linearBufferEntry);
		static unsigned int GetChannelsNum(const SLLibreColorType& colorType);

		void Reset();
		void CreateImageParam(unsigned int cols, unsigned int rows, int pitch, const SLLibreColorType& colorType, const BYTE* linearBufferEntry);
		void CreateImageParam(unsigned int cols, unsigned int rows, unsigned int pitch, const SLLibreColorType& colorType, const BYTE* linearBufferEntry);

		inline unsigned int     Width()             const { return m_Width; }
		inline unsigned int     Height()            const { return m_Height; }
		inline int              Pitch()             const { return m_Pitch; }
		inline int              PitchAbsolute()     const { return std::abs(m_Pitch); }
		inline int              Channels()          const { return GetChannelsNum( ColorType() ); }
		inline SLLibreColorType	ColorType()	        const { return m_ImageColorType; }
		inline const BYTE*      LinearBufferEntry() const { return m_LinearBufferEntry; }
		inline bool             IsImageBufferNull() const { return (m_LinearBufferEntry == nullptr); }
		inline bool             HasAlphaChannel()   const {
			return m_ImageColorType == SLLibreColorType::LibreColorBGRA 
				|| m_ImageColorType == SLLibreColorType::LibreColorRGBA
				|| m_ImageColorType == SLLibreColorType::LibreColorXYZWRGBA;
		}
		inline bool             IsInColorOrder_RGB() const {
			return m_ImageColorType == SLLibreColorType::LibreColorRGB
				|| m_ImageColorType == SLLibreColorType::LibreColorRGBA
				|| m_ImageColorType == SLLibreColorType::LibreColorXYZRGB
				|| m_ImageColorType == SLLibreColorType::LibreColorXYZWRGBA;
		}
		inline bool             IsInColorOrder_BGR() const {
			return m_ImageColorType == SLLibreColorType::LibreColorBGR
				|| m_ImageColorType == SLLibreColorType::LibreColorBGRA;
		}
	private:
		unsigned int m_Width  = 0;
		unsigned int m_Height = 0;
		/// <summary>Or Stride, Length (in Bytes) of an image row in memory: Pitch =  NumRowPixels * BytesPerPixel + Padding </summary>
		/// <remarks>In GDI library, where all DIBs (CImage) are bottom-up, m_Pitch may always be negative <remarks>
		int m_Pitch  = 0; // Negative in CImage, cannot be unsigned int
		SLLibreColorType m_ImageColorType = SLLibreColorType::LibreColorUndefined;

		// When image is loaded into CImage (linear CPU memory), m_LinearBufferEntry is beginning Byte address.
		// Just an EntryAddress, not responsible for allocate/deallocate
		const BYTE* m_LinearBufferEntry = nullptr;


		inline void SetWidth(unsigned int width) { m_Width = width; }
		inline void SetHeight(unsigned int height) { m_Height = height; }
		inline void SetPitch(int pitch) { m_Pitch = pitch; }
		inline void SetImageColorType(SLLibreColorType channels) { m_ImageColorType = channels; }
		inline void SetLinearBufferEntry(const BYTE* entry) { m_LinearBufferEntry = entry; }
	};




} // End of namespace sldip


#endif // __SLIMAGEOBJECT__