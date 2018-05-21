#ifndef __SLMEMORYOBJECT__
#define __SLMEMORYOBJECT__
#pragma once

#include <memory>

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
		void* m_Buffer = nullptr;
		size_t m_MemoryAlignment = 16;
		size_t m_TotalBytes = 0;

		static const size_t SLMemoryAllocationSafetyOffset = 128;// Same type with m_TotalBytes
	};

	//--------------------------- SLMemoryBuffer -----------------------------------------------------------------
	//============================================================================================================
	//--------------------------- SLMemoryObject -----------------------------------------------------------------

	/// <summary>Use SLMemoryBuffer to handle allocate/de-allocate</summary>
	class SLMemoryObject
	{
	public:
		SLMemoryObject();
		virtual ~SLMemoryObject() {};

	protected:
		std::shared_ptr<SLMemoryBuffer> m_uptrBuffer;
	};




} // End of namespace slutil

#endif //__SLMEMORYOBJECT__