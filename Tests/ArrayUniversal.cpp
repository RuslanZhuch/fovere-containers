#include "pch.h"

//import AlgLinear;
import hfog.Alloc;

import fovere.Array.Universal;

using namespace hfog::MemoryUtils::Literals;


template<mem_t alignment, size_t numOfChunks>
using allocPoolExt_t = hfog::Alloc::PoolExt<alignment, numOfChunks, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;

template<mem_t alignment, size_t numOfChunks>
using allocPool_t = hfog::Alloc::Pool<alignment, numOfChunks, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;

template<mem_t alignment, mem_t maxAllocBytes, size_t numOfChunks>
using allocIslandsExt_t = hfog::Alloc::IslandsExt<alignment, maxAllocBytes, numOfChunks, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;

template<mem_t alignment, mem_t maxAllocBytes, size_t numOfChunks>
using allocIslands_t = hfog::Alloc::Islands<alignment, maxAllocBytes, numOfChunks, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;

template<mem_t alignment, mem_t maxAllocBytes>
using allocUnifiedExt_t = hfog::Alloc::UnifiedExt<alignment, maxAllocBytes,hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;

template<mem_t alignment, mem_t maxAllocBytes>
using allocUnified_t = hfog::Alloc::Unified<alignment, maxAllocBytes,hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;


template <typename Alloc, typename Func>
void testWithLocalBuffer(Func func)
{

	Alloc alloc;

	fovere::Array::Universal<Alloc, int> arr(&alloc);

	func(arr);

}

template <typename Alloc, size_t numOfBytes, typename Func>
void testWithExternalBuffer(Func func)
{

	byte_t extBuffer[numOfBytes];
	Alloc alloc(hfog::MemoryBlock(extBuffer, sizeof(extBuffer)));

	fovere::Array::Universal<Alloc, int> arr(&alloc);

	func(arr);

}

template <mem_t maxAllocBytes, size_t numOfChunks, size_t numOfExtBytes, bool universal = true, typename Func>
void testFull(Func func)
{

	constexpr auto minAlignment{ 4_B };
	constexpr auto universalMultiplyer{ 8 };

	testWithLocalBuffer<allocPool_t<maxAllocBytes, numOfChunks>>(func);
	testWithLocalBuffer<allocIslands_t<minAlignment, maxAllocBytes, numOfChunks>>(func);
	if constexpr (universal)
		testWithLocalBuffer<allocUnified_t<minAlignment * universalMultiplyer, maxAllocBytes * universalMultiplyer>>(func);

	testWithExternalBuffer<allocIslandsExt_t<minAlignment, maxAllocBytes, numOfChunks>, numOfExtBytes>(func);
	testWithExternalBuffer<allocPoolExt_t<maxAllocBytes, numOfChunks>, numOfExtBytes>(func);
	if constexpr (universal)
		testWithExternalBuffer<allocUnifiedExt_t<minAlignment * universalMultiplyer, maxAllocBytes * universalMultiplyer>, 
		numOfExtBytes * universalMultiplyer>(func);

}

void tsImplCreation(auto arr)
{
	EXPECT_EQ(arr.getLen(), size_t(0));

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		EXPECT_EQ(arr.append(1), size_t(0));
		EXPECT_EQ(arr.getLen(), size_t(1));
		EXPECT_EQ(arr[0], 1);

		EXPECT_EQ(arr.append(2), size_t(1));
		EXPECT_EQ(arr.getLen(), size_t(2));
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);

		EXPECT_EQ(arr.append(3), size_t(2));
		EXPECT_EQ(arr.getLen(), size_t(3));
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr[2], 3);

		arr.clear();
		EXPECT_EQ(arr.getLen(), size_t(0));

	}

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		EXPECT_EQ(arr.append(1), size_t(0));
		EXPECT_EQ(arr.getLen(), size_t(1));
		EXPECT_EQ(arr[0], 1);

		EXPECT_EQ(arr.append(2), size_t(1));
		EXPECT_EQ(arr.getLen(), size_t(2));
		EXPECT_EQ(arr[1], 2);

		EXPECT_EQ(arr.append(3), size_t(2));
		EXPECT_EQ(arr.getLen(), size_t(3));
		EXPECT_EQ(arr[2], 3);

		arr.remove(1);
		EXPECT_EQ(arr.getLen(), size_t(2));
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 3);

		arr.remove(0);
		EXPECT_EQ(arr.getLen(), size_t(1));
		EXPECT_EQ(arr[0], 3);

		arr.remove(0);
		EXPECT_EQ(arr.getLen(), size_t(0));

	}
}

TEST(ArrayUniversal, tsCreation)
{

	testFull<12_B, 2, 64_B>([](auto arr) {tsImplCreation(arr); });

}

void tsImplRemove(auto arr)
{

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		EXPECT_EQ(arr.append(1), size_t(0));
		EXPECT_EQ(arr.getLen(), size_t(1));
		EXPECT_EQ(arr[0], 1);

		EXPECT_EQ(arr.append(2), size_t(1));
		EXPECT_EQ(arr.getLen(), size_t(2));
		EXPECT_EQ(arr[1], 2);

		EXPECT_EQ(arr.append(3), size_t(2));
		EXPECT_EQ(arr.getLen(), size_t(3));
		EXPECT_EQ(arr[2], 3);

		arr.remove(0);
		EXPECT_EQ(arr.getLen(), size_t(2));
		EXPECT_EQ(arr[0], 2);
		EXPECT_EQ(arr[1], 3);

		arr.remove(1);
		EXPECT_EQ(arr.getLen(), size_t(1));
		EXPECT_EQ(arr[0], 2);

		arr.remove(0);
		EXPECT_EQ(arr.getLen(), size_t(0));

	}

}

TEST(ArrayUniversal, tsRemove)
{

	testFull<12_B, 2, 64_B>([](auto arr) {tsImplRemove(arr); });

}

void tsImplRemoveRange(auto arr)
{

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		size_t beginId{ 1 };
		size_t endId{ 4 };

		EXPECT_EQ(arr.append(1), size_t(0));
		EXPECT_EQ(arr.append(2), size_t(1));
		EXPECT_EQ(arr.append(3), size_t(2));
		EXPECT_EQ(arr.append(4), size_t(3));
		EXPECT_EQ(arr.append(5), size_t(4));
		EXPECT_EQ(arr.append(6), size_t(5));
		EXPECT_EQ(arr.getLen(), size_t(6));

		arr.remove(beginId, endId);

		EXPECT_EQ(arr.getLen(), size_t(3));
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 5);
		EXPECT_EQ(arr[2], 6);

		arr.remove(0, 3);
		EXPECT_EQ(arr.getLen(), size_t(0));

	}

}

TEST(ArrayUniversal, tsRemoveRange)
{

	testFull<24_B, 2, 64_B>([](auto arr) {tsImplRemoveRange(arr); });

}

void tsImplInsertion(auto arr)
{

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		for (size_t insertId{ 0 }; insertId < 5; ++insertId)
		{

			EXPECT_EQ(arr.append(1), size_t(0));
			EXPECT_EQ(arr.append(2), size_t(1));
			EXPECT_EQ(arr.append(3), size_t(2));
			EXPECT_EQ(arr.append(4), size_t(3));
			EXPECT_EQ(arr.append(5), size_t(4));

			EXPECT_TRUE(arr.insert(insertId, -1));

			EXPECT_EQ(arr.getLen(), size_t(6));

			for (size_t id{ 0 }; id < 6; ++id)
			{
				if (id <= insertId)
					EXPECT_EQ(arr[id], static_cast<int>(id + 1));
				else if (id > insertId + 1)
					EXPECT_EQ(arr[id], static_cast<int>(id));
				else
					EXPECT_EQ(arr[id], -1);
			}

			arr.clear();

		}

	}

}

TEST(ArrayUniversal, tsInsertion)
{

	testFull<24_B, 2, 64_B>([](auto arr) {tsImplInsertion(arr); });

}

void tsImplLimits(auto arr)
{

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		EXPECT_EQ(arr.append(1), size_t(0));
		EXPECT_EQ(arr.append(2), size_t(1));
		EXPECT_EQ(arr.append(3), fovere::invalidIndex);
		EXPECT_FALSE(arr.insert(0, 4));

		EXPECT_EQ(arr.getLen(), size_t(2));

		arr.clear();

	}

}

TEST(ArrayUniversal, tsLimits)
{

	testFull<8_B, 2, 64_B, false>([](auto arr) {tsImplLimits(arr); });

}

void tsImplForEach(auto arr)
{

	EXPECT_EQ(arr.append(1), size_t(0));
	EXPECT_EQ(arr.append(2), size_t(1));
	EXPECT_EQ(arr.append(3), size_t(2));
	EXPECT_EQ(arr.append(4), size_t(3));

	int expected{ 1 };
	for (auto el : arr)
	{
		EXPECT_EQ(el, expected++);
	}
	EXPECT_EQ(expected, 5);

	arr.remove(2);
	for (size_t id{ 0 }; auto el: arr)
	{
		static int expectedData[]{ 1, 2, 4 };
		EXPECT_EQ(el, expectedData[id++]);
	}

	arr.remove(0);
	for (size_t id{ 0 }; auto el: arr)
	{
		static int expectedData[]{ 2, 4 };
		EXPECT_EQ(el, expectedData[id++]);
	}

	arr.remove(1);
	for (size_t id{ 0 }; auto el: arr)
	{
		static int expectedData[]{ 2 };
		EXPECT_EQ(el, expectedData[id++]);
	}

	arr.remove(0);
	int numOfIters = 0;
	for ([[maybe_unused]] auto el: arr)
	{
		++numOfIters;
	}
	EXPECT_EQ(numOfIters, 0);

}

TEST(ArrayUniversal, tsForEach)
{

	testFull<32_B, 4, 64_B>([](auto arr) {tsImplForEach(arr); });

}