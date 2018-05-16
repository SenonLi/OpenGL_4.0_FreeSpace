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
		if (m_Buffer != nullptr) {
			_mm_free(m_Buffer);
			m_InstanceCounter--;

#ifdef _DEBUG
			//OutputDebugString(_T("SLMemoryBuffer::AllocateBuffer() %d"))
#endif
		}
	}

	/// <summary> allocate Aligned Memory </summary>
	void SLMemoryBuffer::AllocateAlignedMemory()
	{
		//assert(m_TotalBytes > 0 && m_Buffer == nullptr)

		// Make sure m_TotalBytes > 0 when doing an actual allocation
		if (m_TotalBytes > 0)
		{
			m_InstanceCounter++;

			m_Buffer = _mm_malloc(m_TotalBytes + SLMemoryAllocationSafetyOffset, m_MemoryAlignment);

#ifdef _DEBUG
			//OutputDebugString(_T("SLMemoryBuffer::AllocateBuffer() %d"))
#endif
		}
	}

	//--------------------------- SLMemoryBuffer -----------------------------------------------------------------
	//============================================================================================================
	//--------------------------- SLMemoryObject -----------------------------------------------------------------

	SLMemoryObject::SLMemoryObject()
	{
	}


	SLMemoryObject::~SLMemoryObject()
	{
	}


} // End of namespace slutil
