#include "../stdafx.h"
#include "SLSharedMemoryObject.h"

#include "emmintrin.h" // for SSE aligned memory allocation/deallocation

namespace slutil
{
	//============================================================================================================
	//--------------------------- SLAlignedMemoryBuffer -----------------------------------------------------------------

	int SLAlignedMemoryBuffer::m_InstanceCounter = 0; // Declaration of private static counter

	SLAlignedMemoryBuffer::SLAlignedMemoryBuffer(unsigned long long totalBytes)
	{
		AllocateAlignedMemory(totalBytes);
	}

	SLAlignedMemoryBuffer::~SLAlignedMemoryBuffer()
	{
		if (m_BufferEntry != nullptr) 
		{
			_mm_free(m_BufferEntry);
			m_InstanceCounter--;

			assert(m_InstanceCounter >= 0);
			std::wstringstream outputMessage;
			outputMessage << _T("SLAlignedMemoryBuffer::~SLAlignedMemoryBuffer(), There exist now \t") << m_InstanceCounter << _T(" \t SLAlignedMemoryBuffer allocated.\n");
			SLOutputDebugString(outputMessage.str().c_str());
		}
	}

	/// <summary> allocate Aligned Memory </summary>
	void SLAlignedMemoryBuffer::AllocateAlignedMemory(unsigned long long totalBytes)
	{
		assert(totalBytes > 0 && m_BufferEntry == nullptr);

		// Make sure m_TotalBytes > 0 when doing an actual allocation
		m_InstanceCounter++;
		m_BufferEntry = static_cast<BYTE*>( _mm_malloc(totalBytes + SL_SSE_MemoryAllocationSafetyOffset, SL_SSE_MemoryAlignmentSize) );

		std::wstringstream outputMessage;
		outputMessage << _T("SLAlignedMemoryBuffer::AllocateAlignedMemory(), There exist now \t")
			<< m_InstanceCounter << _T(" \t SLAlignedMemoryBuffer allocated.\n");
		SLOutputDebugString(outputMessage.str().c_str());
	}

	//--------------------------- SLAlignedMemoryBuffer -----------------------------------------------------------------
	//============================================================================================================
	//--------------------------- SLSharedMemoryObject -----------------------------------------------------------------

	SLSharedMemoryObject::SLSharedMemoryObject(unsigned long long totalBytes)
	{
		m_SharedBuffer = std::make_shared<SLAlignedMemoryBuffer>(totalBytes);
	}


} // End of namespace slutil
