#include "pch.h"
#include <iostream>

import hfog.Sources.Local;
import hfog.Sources.Ext;

import hfog.Algorithms.Pool;

import fovere.Array.StaticStable;

using namespace hfog::MemoryUtils::Literals;

template <mem_t bufferSize, size_t numOfElements, typename Func>
void testWithLocalBuffer(Func func)
{

	using elType_t = int;

	using source_t = hfog::Sources::Local<bufferSize, hfog::GarbageWriter::Default>;
//	using allocPool_t = hfog::Algorithms::Pool<source_t, sizeof(elType_t), numOfElements>;

//	allocPool_t alloc;

	fovere::Array::StaticStable<source_t, elType_t, numOfElements> arr;


//	std::cout << "LocalBuffer test\n";
	func(std::move(arr));

}

template <mem_t bufferSize, size_t numOfElements, typename Func>
void testWithExternalBuffer(Func func)
{

	using elType_t = int;

	byte_t extBuffer[bufferSize];
	using source_t = hfog::Sources::External<hfog::GarbageWriter::Default>;
//	using allocPool_t = hfog::Algorithms::Pool<source_t, sizeof(elType_t), numOfElements>;

//	allocPool_t alloc(hfog::MemoryBlock(extBuffer, sizeof(extBuffer)));

	fovere::Array::StaticStable<source_t, elType_t, numOfElements> arr(hfog::MemoryBlock(extBuffer, sizeof(extBuffer)));

//	std::cout << "ExtBuffer test\n";
	func(std::move(arr));

}

template <mem_t bufferSize, size_t numOfElements, typename Func>
void testFull(Func func)
{

	testWithLocalBuffer<bufferSize, numOfElements>(func);

	testWithExternalBuffer<bufferSize, numOfElements>(func);

}

template <size_t numOfElements>
void tsImplCreation(auto&& arr)
{

	for (size_t nElements{ 0 }; nElements <= numOfElements; ++nElements)
	{
		EXPECT_EQ(arr.getLen(), nElements);
		for (size_t elId{ 0 }; elId < nElements; ++elId)
		{
			EXPECT_EQ(arr[elId], static_cast<int>(elId + 1));
		}
		const auto expectId = [](size_t currElements)
		{
			return currElements < numOfElements ? currElements : fovere::invalidIndex;
		};
		EXPECT_EQ(arr.append(static_cast<int>(nElements + 1)), expectId(nElements));
	}

}

TEST(ArrayStaticStable, tsCreation)
{

	testFull<16_B, 4>([](auto&& arr) {tsImplCreation<4>(arr); });

}

void tsImplClear(auto&& arr)
{

	for (size_t iterationsLeft{ 3 }; iterationsLeft > 0; --iterationsLeft)
	{
		EXPECT_EQ(arr.append(1), size_t(0));
		EXPECT_EQ(arr.append(2), size_t(1));
		EXPECT_EQ(arr.append(3), size_t(2));
		EXPECT_EQ(arr.append(4), size_t(3));
		EXPECT_EQ(arr.getLen(), size_t(4));

		arr.clear();
	}

}

TEST(ArrayStaticStable, tsClear)
{

	testFull<16_B, 4>([](auto&& arr) {tsImplClear(arr); });

}

void tsImplRemoving(auto&& arr)
{

	for (size_t iterationsLeft{ 1 }; iterationsLeft > 0; --iterationsLeft)
	{

		const auto el0Id{ arr.append(1) };
		const auto el1Id{ arr.append(2) };
		const auto el2Id{ arr.append(3) };
		const auto el3Id{ arr.append(4) };

		arr.remove(el0Id);
		EXPECT_EQ(arr.getLen(), size_t(3));

		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr[2], 3);
		EXPECT_EQ(arr[3], 4);

		arr.remove(el2Id);
		EXPECT_EQ(arr.getLen(), size_t(2));

		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr[3], 4);

		arr.remove(el3Id);
		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr.getLen(), size_t(1));

		arr.remove(el1Id);
		EXPECT_EQ(arr.getLen(), size_t(0));

	}

}

TEST(ArrayStaticStable, tsRemoving)
{

	testFull<16_B, 4>([](auto&& arr) {tsImplRemoving(arr); });

}

template <size_t numOfElements>
void tsImplRemovingAndAppending(auto&& arr)
{

	[[maybe_unused]] auto el0Id{ arr.append(1) };
	[[maybe_unused]] auto el1Id{ arr.append(2) };
	[[maybe_unused]] auto el2Id{ arr.append(3) };
	[[maybe_unused]] auto el3Id{ arr.append(4) };

	int appendValue{ static_cast<int>(numOfElements) };
	for (size_t remElId{ 0 }; remElId < numOfElements; ++remElId)
	{

		arr.remove(remElId);
		EXPECT_EQ(arr.getLen(), numOfElements - 1);

		for (size_t elId{ 0 }; elId < numOfElements; ++elId)
		{

			if (elId < remElId)
				EXPECT_EQ(arr[elId], static_cast<int>(numOfElements + 1 + elId));
			else if (elId != remElId)
				EXPECT_EQ(arr[elId], static_cast<int>(elId + 1));

		}

		EXPECT_EQ(arr.append(++appendValue), remElId);

		for (size_t elId{ 0 }; elId < numOfElements; ++elId)
		{
			if (elId < remElId)
				EXPECT_EQ(arr[elId], static_cast<int>(numOfElements + 1 + elId));
			else if (elId != remElId)
				EXPECT_EQ(arr[elId], static_cast<int>(elId + 1));
			else
				EXPECT_EQ(arr[elId], appendValue);
		}

	}

}

TEST(ArrayStaticStable, tsRemovingAndAppending)
{

	testFull<16_B, 4>([](auto&& arr) {tsImplRemovingAndAppending<4>(arr); });

}

void tsImplRandomAccess(auto&& arr)
{

	auto el0Id{ arr.append(1) };
	auto el1Id{ arr.append(2) };
	auto el2Id{ arr.append(3) };
	auto el3Id{ arr.append(4) };

	arr[0] = 10;
	arr[1] = 20;
	arr[2] = 30;
	arr[3] = 40;
	EXPECT_EQ(arr[0], 10);
	EXPECT_EQ(arr[1], 20);
	EXPECT_EQ(arr[2], 30);
	EXPECT_EQ(arr[3], 40);

}

TEST(ArrayStaticStable, tsRandomAccess)
{

	testFull<16_B, 4>([](auto&& arr) {tsImplRandomAccess(arr); });

}

static void createReversed(auto& arr, size_t numOfElements = 4)
{

	for (int id{ 0 }; id < numOfElements; ++id)
	{
		auto elId{ arr.append(id + 1) };
	}

	for (size_t id{ 0 }; id < numOfElements; ++id)
	{
		arr.remove(numOfElements - id - 1);
	}

	for (int id{ 0 }; id < numOfElements; ++id)
	{
		auto elId{ arr.append(id + 1) };
	}

}

template <typename T>
void tsImplIterationRead(auto&& arr)
{

	createReversed(arr);

	size_t cnt{ 0 };
	for (size_t id{ 0 }; const auto el : arr)
	{
		++cnt;
		EXPECT_EQ(el, static_cast<T>(++id));
	}
	EXPECT_EQ(cnt, size_t(4));

}

template <typename T>
static void tsIterationRead()
{
//	using source_t = hfog::Sources::Local<16_B, hfog::GarbageWriter::Default>;
//
//	static constexpr size_t numOfElements{ 4 };
//	fovere::Array::StaticStable<source_t, T, numOfElements> arr;
//	createReversed(arr);
//
//	size_t cnt{ 0 };
//	for (size_t id{ 0 }; const auto el : arr)
//	{
//		++cnt;
//		EXPECT_EQ(el, static_cast<T>(++id));
//	}
//	EXPECT_EQ(cnt, 4);

	testFull < 16_B, 4>([](auto&& arr) {tsImplIterationRead<T>(arr); });

}

void tsImplIteration(auto&& arr)
{

	createReversed(arr);

	for (size_t id{ 0 }; auto & el : arr)
	{
		el = 10 + ++id;
	}

	for (size_t id{ 0 }; const auto el : arr)
	{
		EXPECT_EQ(el, static_cast<int>(10 + ++id));
	}

}

TEST(ArrayStaticStable, tsIteration)
{

	tsIterationRead<int>();
	tsIterationRead<const int>();

	testFull<16_B, 4>([](auto&& arr) {tsImplIteration(arr); });

}

template <size_t numOfElements>
void tsImplPrecache(auto&& arr)
{

	createReversed(arr, numOfElements);

	arr.precache();

	size_t cnt{ 0 };
	for (size_t id{ 0 }; const auto el : arr)
	{
		++cnt;
		EXPECT_EQ(el, static_cast<int>(numOfElements - id++));
	}
	EXPECT_EQ(cnt, numOfElements);

}

template <size_t numOfElements>
void tsImplPrecacheShuffled(auto&& arr)
{

	createReversed(arr, numOfElements);

	arr.remove(1);
	arr.append(6);

	arr.remove(4);
	arr.append(7);

	arr.precache();

	{
		int expected[]{ 5, 6, 3, 2, 7 };
		size_t cnt{ 0 };
		for (size_t id{ 0 }; const auto el : arr)
		{
			++cnt;
			EXPECT_EQ(el, expected[id++]);
		}
		EXPECT_EQ(cnt, numOfElements);
	}

	arr.remove(2);
	arr.precache();
	{
		int expected[]{ 5, 3, 2, 7 };
		size_t cnt{ 0 };
		for (size_t id{ 0 }; const auto el : arr)
		{
			++cnt;
			EXPECT_EQ(el, expected[id++]);
		}
		EXPECT_EQ(cnt, numOfElements - 1);
	}

}

TEST(ArrayStaticStable, tsPrecache)
{

	using source_t = hfog::Sources::Local<20_B, hfog::GarbageWriter::Default>;

	testFull<20_B, 4>([](auto&& arr) {tsImplPrecache<4>(arr); });

	testFull<20_B, 5>([](auto&& arr) {tsImplPrecache<5>(arr); });

	testFull<20_B, 5>([](auto&& arr) {tsImplPrecacheShuffled<5>(arr); });

}