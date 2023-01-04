#include "Precompiled.h"
#include "BlockAllocator.h"

#include "DebugUtil.h"

using namespace SAVAGE::Core;

BlockAllocator::BlockAllocator(std::size_t blockSize, std::size_t capacity)
	: mBlockSize(blockSize),
	mCapacity(capacity)
{
	mFreeSlot.clear();
	mFreeSlot.reserve(capacity);

	//ASSERT((mBlockSize != 0) && (capacity != 0), "[BlockAllocator] BlockSize cannot be zero");

	for (size_t i = 0; i < capacity; ++i)
	{
		mFreeSlot.push_back(i);
	}
	mData = malloc(mBlockSize * mCapacity);
}

BlockAllocator::~BlockAllocator()
{
	mBlockSize = 0;
	mCapacity = 0;
	mFreeSlot.clear();
	std::free(mData);
}

void* BlockAllocator::Allocate()
{
	if (!mFreeSlot.empty())
	{
		uint32_t nextSlot = mFreeSlot.back();
		mFreeSlot.pop_back();

		void* newData = static_cast<uint8_t*>(mData) + (nextSlot * mBlockSize);
		return newData;
	}
	else
	{
		return nullptr;
	}
}

void BlockAllocator::Free(void* ptr)
{
	ptrdiff_t diff = static_cast<uint8_t*>(ptr) - mData;

	//ASSERT(distance >= 0 && distance <= static_cast<ptrdiff_t>((mBlockSize * mCapacity) - mBlockSize), "[BlockAllocator] distance between addresses cannot be negative or more than the size of the data");

	std::size_t returnSlot = static_cast<std::size_t>(diff) / mBlockSize;
	mFreeSlot.push_back(returnSlot);
}
