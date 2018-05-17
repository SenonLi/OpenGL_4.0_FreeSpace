#ifndef __SLMEMORYOBJECT__
#define __SLMEMORYOBJECT__
#pragma once

namespace slutil
{ 
	class SLMemoryBuffer
	{
	public:
		~SLMemoryBuffer();

		void AllocateAlignedMemory();

	private:
		/// <remarks>m_InstanceCounter will only count when </remarks>
		static int m_InstanceCounter;
		unsigned long long m_TotalBytes = 0;
		size_t m_MemoryAlignment = 16;
		void* m_Buffer = nullptr;

		static const int SLMemoryAllocationSafetyOffset = 128;
	};

	//--------------------------- SLMemoryBuffer -----------------------------------------------------------------
	//============================================================================================================
	//--------------------------- SLMemoryObject -----------------------------------------------------------------

	/// <summary>Use SLMemoryBuffer to handle allocate/de-allocate</summary>
	class SLMemoryObject
	{
	public:
		SLMemoryObject();
		virtual ~SLMemoryObject();
	};




} // End of namespace slutil

#endif //__SLMEMORYOBJECT__