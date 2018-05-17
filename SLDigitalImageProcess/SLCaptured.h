#ifndef __SLCAPTURED__
#define __SLCAPTURED__
#pragma once

#include "Memory/SLMemoryObject.h"
#include "SLImageParam.h"

namespace sldip
{
	/// <summary>Self image Allocation with help of SLMemoryObject, specially for live scan image GPU-Process</summary>
	class SLCaptured : public slutil::SLMemoryObject
	{
	public:
		virtual ~SLCaptured() {};

	private:
		SLImageParam m_ImageParam; // Contains basic image info of SLCaptured
	};



} // End of namespace sldip

#endif
