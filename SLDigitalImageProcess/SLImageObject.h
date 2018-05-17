#ifndef __SLIMAGEOBJECT__
#define __SLIMAGEOBJECT__
#pragma once

#include "Memory/SLMemoryObject.h"

class SLImageObject : public slutil::SLMemoryObject
{
public:
	inline int Width()      const { return m_Width; }
	inline int Height()     const { return m_Height; }
	inline int Channels()	const { return m_Channels; }

private:
	int m_Width = 0;
	int m_Height = 0;
	int m_Channels = 0;
};


#endif // __SLIMAGEOBJECT__