#include "../stdafx.h"
#include "SLSharedMemoryObject.h"

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
			free(m_BufferEntry);
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
		// Make sure m_TotalBytes > 0 when doing an actual allocation
		assert(totalBytes > 0 && m_BufferEntry == nullptr);

		m_InstanceCounter++;

		// It won't hurt to allocate a bit more, and will help a lot preventing potential crash
		unsigned int alignedMemoryAllocationSafetyOffset = 16; // Default Alignment on 64bit
		m_BufferEntry = static_cast<BYTE*>(malloc(static_cast<size_t>(totalBytes + alignedMemoryAllocationSafetyOffset)) );

		std::wstringstream outputMessage;
		outputMessage << _T("SLAlignedMemoryBuffer::AllocateAlignedMemory(), There exist now \t")
			<< m_InstanceCounter << _T(" \t SLAlignedMemoryBuffer allocated.\n");
		SLOutputDebugString(outputMessage.str().c_str());
	}

	//--------------------------- SLAlignedMemoryBuffer -----------------------------------------------------------------
	//============================================================================================================
	//--------------------------- SLSharedMemoryObject -----------------------------------------------------------------
	SLSharedMemoryObject::SLSharedMemoryObject()
	{
		m_SharedBuffer = std::make_shared<SLAlignedMemoryBuffer>();
	}

	SLSharedMemoryObject::SLSharedMemoryObject(unsigned long long totalBytes)
	{
		CreateSharedMemory(totalBytes);
	}

	/// <summary> Will Create directly, no need to call Reset 
	///           Will automatically re-new m_SharedBuffer and m_TotalBytes </summary>
	void SLSharedMemoryObject::CreateSharedMemory(unsigned long long totalBytes)
	{
		m_TotalBytes = totalBytes;
		m_SharedBuffer = std::make_shared<SLAlignedMemoryBuffer>(m_TotalBytes);
	}

	/// <summary>Reset SLSharedMemoryObject</summary>
	void SLSharedMemoryObject::Reset()
	{
		m_SharedBuffer.reset();
		m_TotalBytes = 0;
	}


} // End of namespace slutil
