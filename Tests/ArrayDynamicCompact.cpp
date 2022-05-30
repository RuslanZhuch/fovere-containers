#include "pch.h"

//import AlgLinear;
import hfog.Alloc;
import hfog.Sources.Local;
import hfog.Sources.Ext;

import fovere.Array.DynamicCompact;

using namespace hfog::MemoryUtils::Literals;

template<mem_t alignment>
using allocStackExt_t = hfog::Algorithms::Stack<hfog::Sources::External<>, alignment>;

template<mem_t alignment, mem_t bytes>
using allocStack_t = hfog::Algorithms::Stack<hfog::Sources::Local<bytes>, alignment>;

template <mem_t numOfBytes, typename Func>
void testWithLocalBuffer(Func func)
{

	using Alloc = allocStack_t<sizeof(int), numOfBytes>;
	Alloc alloc;

	fovere::Array::DynamicCompact<int, hfog::Sources::Local<numOfBytes>> arr(&alloc);

	func(arr);

}

template <mem_t numOfBytes, typename Func>
void testWithExternalBuffer(Func func)
{

	byte_t extBuffer[numOfBytes];

	using Alloc = allocStackExt_t<sizeof(int)>;
	Alloc alloc(hfog::MemoryBlock(extBuffer, sizeof(extBuffer)));

	fovere::Array::DynamicCompact<int, hfog::Sources::External<>> arr(&alloc);

	func(arr);

}

template <mem_t bytes, size_t numOfExtBytes, typename Func>
void testFull(Func func)
{

	testWithLocalBuffer<bytes>(func);
	testWithExternalBuffer<numOfExtBytes>(func);

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

TEST(ArrayDynamicCompact, tsCreation)
{

	testFull<12_B, 64_B>([](auto arr) {tsImplCreation(arr); });

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

TEST(ArrayDynamicCompact, tsRemove)
{

	testFull<12_B, 64_B>([](auto arr) {tsImplRemove(arr); });

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

TEST(ArrayDynamicCompact, tsRemoveRange)
{

	testFull<24_B, 64_B>([](auto arr) {tsImplRemoveRange(arr); });

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

TEST(ArrayDynamicCompact, tsInsertion)
{

	testFull<24_B, 64_B>([](auto arr) {tsImplInsertion(arr); });

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

TEST(ArrayDynamicCompact, tsLimits)
{

	testFull<8_B, 8_B>([](auto arr) {tsImplLimits(arr); });

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

TEST(ArrayDynamicCompact, tsForEach)
{

	testFull<16_B, 16_B>([](auto arr) {tsImplForEach(arr); });

}