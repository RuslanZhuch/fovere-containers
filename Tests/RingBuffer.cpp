#include "pch.h"

//import AlgLinear;
import hfog.Algorithms.Ring;
import hfog.Sources.Local;
import hfog.Sources.Ext;

import fovere.RingBuffer;

using namespace hfog::MemoryUtils::Literals;

template<mem_t alignment>
using allocRingExt_t = hfog::Algorithms::Ring<hfog::Sources::External<>, alignment>;

template<mem_t alignment, mem_t bytes>
using allocRing_t = hfog::Algorithms::Ring<hfog::Sources::Local<bytes>, alignment>;

static constexpr size_t MAX_ELEMENTS{ 10 };

template <mem_t numOfBytes, typename Func>
void testWithLocalBuffer(Func func)
{

	using Alloc = allocRing_t<sizeof(int), numOfBytes>;
	Alloc alloc;

	fovere::Buffer::Ring<int, MAX_ELEMENTS, Alloc> arr(&alloc, numOfBytes);

	func(arr);
	
}

template <mem_t numOfBytes, typename Func>
void testWithExternalBuffer(Func func)
{

	byte_t extBuffer[numOfBytes];

	using Alloc = allocRingExt_t<sizeof(int)>;
	Alloc alloc(hfog::MemoryBlock(extBuffer, sizeof(extBuffer)));

	fovere::Buffer::Ring<int, MAX_ELEMENTS, Alloc> arr(&alloc, numOfBytes);

	func(arr);

}

template <mem_t bytes, size_t numOfExtBytes, typename Func>
void testFull(Func func)
{

	testWithLocalBuffer<bytes>(func);
	testWithExternalBuffer<numOfExtBytes>(func);

}

void tsImplCreation(auto&& arr)
{

	EXPECT_EQ(arr.getLen(), size_t(0));
	static constexpr auto NUM_OF_ELEMENTS{ 3 };

	for (size_t itersLeft{ 10 }; itersLeft > 0; --itersLeft)
	{

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{

			auto value{ arr.append() };
			EXPECT_NE(value, nullptr);
			*value = elementId + 1;
			EXPECT_EQ(arr.getLen(), size_t(elementId + 1));

		}

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{
			int data{};
			EXPECT_EQ(arr.copyAndPop(&data, sizeof(data)), sizeof(data));
			EXPECT_EQ(data, elementId + 1);
			EXPECT_EQ(arr.getLen(), size_t(NUM_OF_ELEMENTS - elementId - 1));
		}

	}

}

TEST(RingBuffer, tsCreation)
{

	testFull<32_B, 32_B>([](auto&& arr) {tsImplCreation(arr); });

}

void tsImplPop(auto&& arr)
{

	EXPECT_EQ(arr.getLen(), size_t(0));
	static constexpr auto NUM_OF_ELEMENTS{ 3 };

	for (size_t itersLeft{ 10 }; itersLeft > 0; --itersLeft)
	{

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{

			auto value{ arr.append() };
			EXPECT_NE(value, nullptr);
			*value = elementId + 1;
			EXPECT_EQ(arr.getLen(), size_t(elementId + 1));

		}

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{
			arr.pop();
			EXPECT_EQ(arr.getLen(), size_t(NUM_OF_ELEMENTS - elementId - 1));
		}

	}

}

TEST(RingBuffer, tsPop)
{

	testFull<32_B, 32_B>([](auto&& arr) {tsImplPop(arr); });

}

void tsImplRemoving(auto&& arr)
{

	EXPECT_EQ(arr.getLen(), size_t(0));
	static constexpr auto NUM_OF_ELEMENTS{ 4 };

	for (size_t itersLeft{ 10 }; itersLeft > 0; --itersLeft)
	{

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{
			auto value{ arr.append() };
			EXPECT_NE(value, nullptr);
			*value = elementId + 1;
			EXPECT_EQ(arr.getLen(), size_t(elementId + 1));
		}

		arr.clear();

	}

}

TEST(RingBuffer, tsRemoving)
{

	testFull<32_B, 32_B>([](auto&& arr) {tsImplRemoving(arr); });

}

void tsImplRawData(auto&& arr)
{

	EXPECT_EQ(arr.getLen(), size_t(0));
	static constexpr auto NUM_OF_ELEMENTS{ 1 };

	int data[16];


	const auto checkData = [](auto&& data)
	{
		using Type = decltype(data);
		for (int id{ 0 }; id < sizeof(data) / sizeof(Type); ++id)
		{
			EXPECT_EQ(data[id], 10 * id + 1);
		}
	};

	for (size_t itersLeft{ 10 }; itersLeft > 0; --itersLeft)
	{

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{
			auto data{ arr.append(16) };
			EXPECT_NE(data, nullptr);
			for (int id{ 0 }; id < 16; ++id)
			{
				data[id] = 10 * id + 1;
			}
			EXPECT_EQ(arr.getLen(), size_t(elementId + 1));
		}

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{
			EXPECT_EQ(arr.copyAndPop(data, sizeof(data)), sizeof(data));
			checkData(data);
			EXPECT_EQ(arr.getLen(), size_t(NUM_OF_ELEMENTS - elementId - 1));
		}

	}

}

TEST(RingBuffer, tsRawData)
{

	testFull<256_B, 256_B>([](auto&& arr) {tsImplRawData(arr); });

}

void tsImplOutOfMem(auto&& arr)
{

	for (size_t itersLeft{ 5 }; itersLeft > 0; --itersLeft)
	{

		auto valueOutOfMem{ arr.append(36) };
		EXPECT_EQ(valueOutOfMem, nullptr);
		
		auto value{ arr.append(16) };
		EXPECT_NE(value, nullptr);
		
		int data[16];
		EXPECT_EQ(arr.copyAndPop(data, sizeof(data)), sizeof(data));

	}

}

TEST(RingBuffer, tsOutOfMem)
{

	testFull<64_B, 64_B>([](auto&& arr) {tsImplOutOfMem(arr); });

}

void tsImplFull(auto&& arr)
{

	for (size_t itersLeft{ 5 }; itersLeft > 0; --itersLeft)
	{

		for (size_t elId{ 0 }; elId < MAX_ELEMENTS; ++elId)
		{
			auto value{ arr.append(2) };
			EXPECT_NE(value, nullptr);
		}

		auto valueOutOfLimits{ arr.append(2) };
		EXPECT_EQ(valueOutOfLimits, nullptr);

		for (size_t elId{ 0 }; elId < MAX_ELEMENTS; ++elId)
		{
			int data[2];
			EXPECT_EQ(arr.copyAndPop(data, sizeof(data)), sizeof(data));
		}

	}

}

TEST(RingBuffer, tsFull)
{

	testFull<96_B, 96_B>([](auto&& arr) {tsImplFull(arr); });

}

void tsImplForEach(auto&& arr)
{

	static constexpr auto NUM_OF_ELEMENTS{ 3 };

	for (size_t itersLeft{ 1 }; itersLeft > 0; --itersLeft)
	{

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{

			auto value{ arr.append() };
			EXPECT_NE(value, nullptr);
			*value = elementId + 1;
			EXPECT_EQ(arr.getLen(), size_t(elementId + 1));

		}

		int expectedElement{ 1 };
		for (auto element : arr)
		{
			EXPECT_EQ(element, expectedElement++);
		}

		EXPECT_EQ(expectedElement, 4);

	}

}

TEST(RingBuffer, tsForEach)
{

	testFull<16_B, 16_B>([](auto&& arr) {tsImplForEach(arr); });

}