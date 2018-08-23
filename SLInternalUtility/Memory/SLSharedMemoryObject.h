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
		SLAlignedMemoryBuffer() {};
		explicit SLAlignedMemoryBuffer(unsigned long long totalBytes, unsigned int alignment);
		~SLAlignedMemoryBuffer();
		void AllocateAlignedMemory(unsigned long long totalBytes, unsigned int alignment);

		BYTE* m_BufferEntry = nullptr;

		/// <remarks>m_InstanceCounter counts whenever SLAlignedMemoryBuffer Allocated/Freed</remarks>
		static int m_InstanceCounter;	
	};

	//--------------------------- SLAlignedMemoryBuffer -----------------------------------------------------------------
	//============================================================================================================
	//--------------------------- SLSharedMemoryObject -----------------------------------------------------------------

	/// <summary>Use SLAlignedMemoryBuffer to automatically handle allocate/de-allocate</summary>
	class SLSharedMemoryObject
	{
	public:
		SLSharedMemoryObject(); // Default constructor here for class member initialization
		explicit SLSharedMemoryObject(unsigned long long totalBytes, unsigned int alignment);
		virtual ~SLSharedMemoryObject() {};

		virtual void Reset();

		inline bool IsNull() const { return (m_SharedBuffer && m_SharedBuffer->m_BufferEntry == nullptr); }
		BYTE* GetBufferEntryForEdit() { return m_SharedBuffer->m_BufferEntry; }
		const BYTE* GetBufferEntry() const { return const_cast<const BYTE*>(m_SharedBuffer->m_BufferEntry); }
		const unsigned long long GetTotalBytes() const { return m_TotalBytes; }

	protected:
		std::shared_ptr<SLAlignedMemoryBuffer> m_SharedBuffer;
		unsigned long long m_TotalBytes = 0;

		void CreateSharedMemory(unsigned long long totalBytes, unsigned int alignment);
	};


} // End of namespace slutil

#endif //__SLSHAREDMEMORYOBJECT__