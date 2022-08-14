#include "pch.h"

//import AlgLinear;
import hfog.Algorithms.Unified;
import hfog.Sources.Local;
import hfog.Sources.Ext;
import hfog.Alloc;

import fovere.Transport.Queue;

using namespace hfog::MemoryUtils::Literals;

template<mem_t alignment>
using allocRingExt_t = hfog::Alloc::UnifiedExt<alignment, 512_B>;

template<mem_t alignment, mem_t bytes>
using allocRing_t = hfog::Alloc::Unified<alignment, 512_B>;

static constexpr size_t MAX_ELEMENTS{ 10 };

union DataStruct
{
	int32_t i{ 0 };
	int64_t p[2];
};

using type_t = DataStruct;

template <mem_t numOfElements, typename Func>
void testWithLocalBuffer(Func func)
{

	constexpr auto numOfBytes{ numOfElements * 32 };
	using Alloc = allocRing_t<sizeof(type_t), 128>;
	Alloc alloc;

	fovere::Transport::Queue<type_t, Alloc> arr(&alloc, numOfElements);

	func(arr);

}

template <mem_t numOfElements, typename Func>
void testWithExternalBuffer(Func func)
{

	constexpr auto numOfBytes{ 512_B + 64_B};
	byte_t extBuffer[numOfBytes];

	using Alloc = allocRingExt_t<150>;
	Alloc alloc(hfog::MemoryBlock(extBuffer, sizeof(extBuffer)));

	fovere::Transport::Queue<type_t, Alloc> arr(&alloc, numOfElements);

	func(arr);

}

template <size_t numOfElements, typename Func>
void testFull(Func func)
{

//	testWithLocalBuffer<numOfElements>(func);
	testWithExternalBuffer<numOfElements>(func);

}

void tsImplCreation(auto&& arr)
{

	EXPECT_EQ(arr.getLen(), size_t(0));
	static constexpr auto NUM_OF_ELEMENTS{ 3 };

	for (size_t itersLeft{ 10 }; itersLeft > 0; --itersLeft)
	{

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{

			const type_t pushEl{ elementId + 1 };
			EXPECT_TRUE(arr.push(pushEl));
			EXPECT_EQ(arr.getLen(), size_t(elementId + 1));

		}

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{
			type_t data{};
			EXPECT_TRUE(arr.copyAndPop(&data));
			EXPECT_EQ(data.i, elementId + 1);
			EXPECT_EQ(arr.getLen(), size_t(NUM_OF_ELEMENTS - elementId - 1));
		}

	}

}

TEST(TransportQueue, tsCreation)
{

	testFull<4>([](auto&& arr) {tsImplCreation(arr); });

}

void tsImplOneAtATime(auto&& arr)
{

	EXPECT_EQ(arr.getLen(), size_t(0));
	static constexpr auto NUM_OF_ITERS{ 100 };

	for (int iterId{ 0 }; iterId < NUM_OF_ITERS; ++iterId)
	{

		EXPECT_TRUE(arr.push({ iterId + 1 }));
		EXPECT_EQ(arr.getLen(), size_t(1));

		type_t data{};
		EXPECT_TRUE(arr.copyAndPop(&data));
		EXPECT_EQ(data.i, iterId + 1);
		EXPECT_EQ(arr.getLen(), size_t(0));

	}

}

TEST(TransportQueue, tsOneAtATime)
{

	testFull<4>([](auto&& arr) {tsImplOneAtATime(arr); });

}


void tsImplSeparateCopyAndPop(auto&& arr)
{

	EXPECT_EQ(arr.getLen(), size_t(0));
	static constexpr auto NUM_OF_ELEMENTS{ 3 };

	for (size_t itersLeft{ 10 }; itersLeft > 0; --itersLeft)
	{

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{

			const type_t pushEl{ elementId + 1 };
			EXPECT_TRUE(arr.push(pushEl));
			EXPECT_EQ(arr.getLen(), size_t(elementId + 1));

		}

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{
			type_t data{};
			EXPECT_TRUE(arr.copy(&data));
			EXPECT_EQ(data.i, elementId + 1);
			EXPECT_EQ(arr.getLen(), size_t(NUM_OF_ELEMENTS - elementId));
			EXPECT_TRUE(arr.copy(&data));
			EXPECT_EQ(data.i, elementId + 1);
			EXPECT_EQ(arr.getLen(), size_t(NUM_OF_ELEMENTS - elementId));
			arr.pop();
			EXPECT_EQ(arr.getLen(), size_t(NUM_OF_ELEMENTS - elementId - 1));
		}

	}

}

TEST(TransportQueue, tsSeparateCopyAndPop)
{

	testFull<4>([](auto&& arr) {tsImplOneAtATime(arr); });

}

//void tsImplOneAtATimeInverse(auto&& arr)
//{
//
//	EXPECT_EQ(arr.getLen(), size_t(0));
//	static constexpr auto NUM_OF_ITERS{ 100 };
//
//	for (int iterId{ 0 }; iterId < NUM_OF_ITERS; ++iterId)
//	{
//
//		type_t data{};
//		if (iterId == 0)
//		{
//			EXPECT_FALSE(arr.copyAndPop(&data));
//		}
//		else
//		{
//			EXPECT_TRUE(arr.copyAndPop(&data));
//			EXPECT_EQ(data.i, iterId);
//			EXPECT_EQ(arr.getLen(), size_t(0));
//		}
//
//		EXPECT_TRUE(arr.push({ iterId + 1 }));
//		EXPECT_EQ(arr.getLen(), size_t(1));
//
//
//	}
//
//}
//
//TEST(TransportQueue, tsOneAtATimeInverse)
//{
//
//	testFull<4>([](auto&& arr) {tsImplOneAtATimeInverse(arr); });
//
//}

void tsImplBottleneck(auto&& arr)
{

	EXPECT_EQ(arr.getLen(), size_t(0));
	static constexpr auto NUM_OF_ELEMENTS{ 4 };
	static constexpr auto NUM_OF_ITERS{ 100 };

	for (size_t iterId{ 0 }; iterId < NUM_OF_ITERS; ++iterId)
	{

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{

			const type_t pushEl{ elementId + 1 };
			EXPECT_TRUE(arr.push(pushEl));
			EXPECT_EQ(arr.getLen(), size_t(elementId + 1));

		}

		EXPECT_FALSE(arr.push({ NUM_OF_ELEMENTS + 1 }));

		type_t data{};
		EXPECT_TRUE(arr.copyAndPop(&data));
		EXPECT_EQ(data.i, 1);

		EXPECT_TRUE(arr.push({ NUM_OF_ELEMENTS + 1 }));

		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
		{

			type_t data{};
			EXPECT_TRUE(arr.copyAndPop(&data));
			EXPECT_EQ(data.i, elementId + 2);
			EXPECT_EQ(arr.getLen(), size_t(NUM_OF_ELEMENTS - elementId - 1));

		}

	}

}

TEST(TransportQueue, tsBottleneck)
{

	testFull<4>([](auto&& arr) {tsImplBottleneck(arr); });

}

//void tsImplRemoving(auto&& arr)
//{
//
//	EXPECT_EQ(arr.getLen(), size_t(0));
//	static constexpr auto NUM_OF_ELEMENTS{ 4 };
//
//	for (size_t itersLeft{ 10 }; itersLeft > 0; --itersLeft)
//	{
//
//		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
//		{
//			auto value{ arr.append() };
//			EXPECT_NE(value, nullptr);
//			*value = elementId + 1;
//			EXPECT_EQ(arr.getLen(), size_t(elementId + 1));
//		}
//
//		arr.clear();
//
//	}
//
//}
//
//TEST(TransportQueue, tsRemoving)
//{
//
//	testFull<32_B, 32_B>([](auto&& arr) {tsImplRemoving(arr); });
//
//}
//
//void tsImplRawData(auto&& arr)
//{
//
//	EXPECT_EQ(arr.getLen(), size_t(0));
//	static constexpr auto NUM_OF_ELEMENTS{ 1 };
//
//	int data[16];
//
//
//	const auto checkData = [](auto&& data)
//	{
//		using Type = decltype(data);
//		for (int id{ 0 }; id < sizeof(data) / sizeof(Type); ++id)
//		{
//			EXPECT_EQ(data[id], 10 * id + 1);
//		}
//	};
//
//	for (size_t itersLeft{ 10 }; itersLeft > 0; --itersLeft)
//	{
//
//		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
//		{
//			auto data{ arr.append(16) };
//			EXPECT_NE(data, nullptr);
//			for (int id{ 0 }; id < 16; ++id)
//			{
//				data[id] = 10 * id + 1;
//			}
//			EXPECT_EQ(arr.getLen(), size_t(elementId + 1));
//		}
//
//		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
//		{
//			EXPECT_EQ(arr.copyAndPop(data, sizeof(data)), sizeof(data));
//			checkData(data);
//			EXPECT_EQ(arr.getLen(), size_t(NUM_OF_ELEMENTS - elementId - 1));
//		}
//
//	}
//
//}
//
//TEST(TransportQueue, tsRawData)
//{
//
//	testFull<256_B, 256_B>([](auto&& arr) {tsImplRawData(arr); });
//
//}
//
//void tsImplOutOfMem(auto&& arr)
//{
//
//	for (size_t itersLeft{ 5 }; itersLeft > 0; --itersLeft)
//	{
//
//		auto valueOutOfMem{ arr.append(36) };
//		EXPECT_EQ(valueOutOfMem, nullptr);
//
//		auto value{ arr.append(16) };
//		EXPECT_NE(value, nullptr);
//
//		int data[16];
//		EXPECT_EQ(arr.copyAndPop(data, sizeof(data)), sizeof(data));
//
//	}
//
//}
//
//TEST(TransportQueue, tsOutOfMem)
//{
//
//	testFull<64_B, 64_B>([](auto&& arr) {tsImplOutOfMem(arr); });
//
//}
//
//void tsImplFull(auto&& arr)
//{
//
//	for (size_t itersLeft{ 5 }; itersLeft > 0; --itersLeft)
//	{
//
//		for (size_t elId{ 0 }; elId < MAX_ELEMENTS; ++elId)
//		{
//			auto value{ arr.append(2) };
//			EXPECT_NE(value, nullptr);
//		}
//
//		auto valueOutOfLimits{ arr.append(2) };
//		EXPECT_EQ(valueOutOfLimits, nullptr);
//
//		for (size_t elId{ 0 }; elId < MAX_ELEMENTS; ++elId)
//		{
//			int data[2];
//			EXPECT_EQ(arr.copyAndPop(data, sizeof(data)), sizeof(data));
//		}
//
//	}
//
//}
//
//TEST(TransportQueue, tsFull)
//{
//
//	testFull<96_B, 96_B>([](auto&& arr) {tsImplFull(arr); });
//
//}
//
//void tsImplForEach(auto&& arr)
//{
//
//	static constexpr auto NUM_OF_ELEMENTS{ 3 };
//
//	for (size_t itersLeft{ 1 }; itersLeft > 0; --itersLeft)
//	{
//
//		for (int elementId{ 0 }; elementId < NUM_OF_ELEMENTS; ++elementId)
//		{
//
//			auto value{ arr.append() };
//			EXPECT_NE(value, nullptr);
//			*value = elementId + 1;
//			EXPECT_EQ(arr.getLen(), size_t(elementId + 1));
//
//		}
//
//		int expectedElement{ 1 };
//		for (auto element : arr)
//		{
//			EXPECT_EQ(element, expectedElement++);
//		}
//
//		EXPECT_EQ(expectedElement, 4);
//
//	}
//
//}
//
//TEST(TransportQueue, tsForEach)
//{
//
//	testFull<16_B, 16_B>([](auto&& arr) {tsImplForEach(arr); });
//
//}