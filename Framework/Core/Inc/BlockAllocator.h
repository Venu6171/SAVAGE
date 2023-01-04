#pragma once

namespace SAVAGE::Core
{
	class BlockAllocator
	{
	public:
		BlockAllocator(std::size_t blockSize, std::size_t capacity);
		~BlockAllocator();

		BlockAllocator(const BlockAllocator&) = delete;
		BlockAllocator& operator=(const BlockAllocator&) = delete;

		BlockAllocator(BlockAllocator&&) = delete;
		BlockAllocator& operator=(BlockAllocator&&) = delete;

		void* Allocate();
		void Free(void* ptr);

	private:
		std::vector<uint32_t> mFreeSlot;
		void* mData = nullptr;
		std::size_t mBlockSize = 0;
		std::size_t mCapacity = 0;
	};
}

#define MEMPOOL_DECLARE\
	static SAVAGE::Core::BlockAllocator sAllocator;\
	static void* operator new(std::size_t size);\
	static void operator delete(void* ptr);

#define MEMPOOL_DEFINE(Class, Capacity)\
	SAVAGE::Core::BlockAllocator Class::sAllocator(sizeof(Class), Capacity);\
	void* Class::operator new(std::size_t size)\
	{\
		return sAllocator.Allocate();\
	}\
	void Class::operator delete(void* ptr)\
	{\
		sAllocator.Free(ptr);\
	}