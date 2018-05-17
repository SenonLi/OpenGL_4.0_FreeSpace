#include "../stdafx.h"
#include "SLMemoryObject.h"

#include "emmintrin.h" // for SSE aligned memory allocation/deallocation

namespace slutil
{
	//--------------------------- Static in slutil ---------------------------------------------------------------
	//============================================================================================================
	//--------------------------- SLMemoryObject -----------------------------------------------------------------

	int SLMemoryBuffer::m_InstanceCounter = 0; // Declaration of private static counter

	SLMemoryBuffer::~SLMemoryBuffer()
	{
		if (m_Buffer != nullptr) 
		{
			_mm_free(m_Buffer);
			m_InstanceCounter--;

			assert(m_InstanceCounter >= 0);
			std::wstringstream outputMessage;
			outputMessage << _T("SLMemoryBuffer::~SLMemoryBuffer(), There exist now \t") << m_InstanceCounter << _T(" \t SLMemoryBuffer allocated.\n");
			SLOutputDebugString(outputMessage.str().c_str());
		}
	}

	/// <summary> allocate Aligned Memory </summary>
	void SLMemoryBuffer::AllocateAlignedMemory()
	{
		assert(m_TotalBytes > 0 && m_Buffer == nullptr);

		// Make sure m_TotalBytes > 0 when doing an actual allocation
		m_InstanceCounter++;
		m_Buffer = _mm_malloc(m_TotalBytes + SLMemoryAllocationSafetyOffset, m_MemoryAlignment);

		std::wstringstream outputMessage;
		outputMessage << _T("SLMemoryBuffer::AllocateAlignedMemory(), There exist now \t")
			<< m_InstanceCounter << _T(" \t SLMemoryBuffer allocated.\n");
		SLOutputDebugString(outputMessage.str().c_str());
	}

	//--------------------------- SLMemoryBuffer -----------------------------------------------------------------
	//============================================================================================================
	//--------------------------- SLMemoryObject -----------------------------------------------------------------

	SLMemoryObject::SLMemoryObject()
	{
		m_uptrBuffer = std::make_shared<SLMemoryBuffer>();
	}



} // End of namespace slutil
