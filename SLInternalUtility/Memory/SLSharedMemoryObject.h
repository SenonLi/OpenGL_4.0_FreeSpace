#ifndef __SLSHAREDMEMORYOBJECT__
#define __SLSHAREDMEMORYOBJECT__
#pragma once

#include <memory>

namespace slutil
{ 
	/// <summary>SSE aligned memory allocation/deallocation for Image Processing </summary>
	class SLAlignedMemoryBuffer
	{
	public:
		explicit SLAlignedMemoryBuffer(unsigned long long totalBytes);
		~SLAlignedMemoryBuffer();
		void AllocateAlignedMemory(unsigned long long totalBytes);

		BYTE* m_BufferEntry = nullptr;

		static const unsigned int SL_SSE_MemoryAllocationSafetyOffset = 128;// Same type with m_TotalBytes
		static const unsigned int SL_SSE_MemoryAlignmentSize = 16;

		/// <remarks>m_InstanceCounter will only count when </remarks>
		static int m_InstanceCounter;	
	};

	//--------------------------- SLAlignedMemoryBuffer -----------------------------------------------------------------
	//============================================================================================================
	//--------------------------- SLSharedMemoryObject -----------------------------------------------------------------

	/// <summary>Use SLAlignedMemoryBuffer to automatically handle allocate/de-allocate</summary>
	class SLSharedMemoryObject
	{
	public:
		explicit SLSharedMemoryObject(unsigned long long totalBytes);
		virtual ~SLSharedMemoryObject() {};

		inline bool IsNull() const { return (m_SharedBuffer && m_SharedBuffer->m_BufferEntry == nullptr); }
		unsigned char* GetBufferEntry() { return m_SharedBuffer->m_BufferEntry; }

	protected:
		std::shared_ptr<SLAlignedMemoryBuffer> m_SharedBuffer;
		unsigned long long m_TotalBytes = 0;
	};


} // End of namespace slutil

#endif //__SLSHAREDMEMORYOBJECT__