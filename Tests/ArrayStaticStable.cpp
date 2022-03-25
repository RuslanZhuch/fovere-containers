#include "pch.h"

import SourceStack;
import fovere.Array.StaticStable;

using namespace hfog::MemoryUtils::Literals;

TEST(ArrayStaticStable, tsCreation)
{

	using source_t = hfog::Sources::Stack<16_B, hfog::GarbageWriter::Default>;

	{

		static constexpr size_t numOfElements{ 4 };
		fovere::Array::StaticStable<source_t, int, numOfElements> arr;

		for (size_t nElements{ 0 }; nElements <= numOfElements; ++nElements)
		{
			EXPECT_EQ(arr.getLen(), nElements);
			for (size_t elId{ 0 }; elId < nElements; ++elId)
			{
				EXPECT_EQ(arr[elId], elId + 1);
			}
			const auto expectId = [](size_t currElements)
			{
				return currElements < numOfElements ? currElements : fovere::invalidIndex;
			};
			EXPECT_EQ(arr.append(static_cast<int>(nElements + 1)), expectId(nElements));
		}
		
	}

}

TEST(ArrayStaticStable, tsClear)
{

	using source_t = hfog::Sources::Stack<16_B, hfog::GarbageWriter::Default>;

	static constexpr size_t numOfElements{ 4 };
	fovere::Array::StaticStable<source_t, int, numOfElements> arr;

	for (size_t iterationsLeft{ 3 }; iterationsLeft > 0; --iterationsLeft)
	{
		EXPECT_EQ(arr.append(1), 0);
		EXPECT_EQ(arr.append(2), 1);
		EXPECT_EQ(arr.append(3), 2);
		EXPECT_EQ(arr.append(4), 3);
		EXPECT_EQ(arr.getLen(), 4);

		arr.clear();
	}

}

TEST(ArrayStaticStable, tsRemoving)
{

	using source_t = hfog::Sources::Stack<16_B, hfog::GarbageWriter::Default>;

	static constexpr size_t numOfElements{ 4 };
	fovere::Array::StaticStable<source_t, int, numOfElements> arr;
	const auto el0Id{ arr.append(1) };
	const auto el1Id{ arr.append(2) };
	const auto el2Id{ arr.append(3) };
	const auto el3Id{ arr.append(4) };

	arr.remove(el0Id);
	EXPECT_EQ(arr.getLen(), 3);

	EXPECT_EQ(arr[1], 2);
	EXPECT_EQ(arr[2], 3);
	EXPECT_EQ(arr[3], 4);

	arr.remove(el2Id);
	EXPECT_EQ(arr.getLen(), 2);

	EXPECT_EQ(arr[1], 2);
	EXPECT_EQ(arr[3], 4);

	arr.remove(el3Id);
	EXPECT_EQ(arr[1], 2);
	EXPECT_EQ(arr.getLen(), 1);

	arr.remove(el1Id);
	EXPECT_EQ(arr.getLen(), 0);

}

TEST(ArrayStaticStable, tsRemovingAndAppending)
{

	using source_t = hfog::Sources::Stack<16_B, hfog::GarbageWriter::Default>;

	static constexpr size_t numOfElements{ 4 };
	fovere::Array::StaticStable<source_t, int, numOfElements> arr;
	auto el0Id{ arr.append(1) };
	auto el1Id{ arr.append(2) };
	auto el2Id{ arr.append(3) };
	auto el3Id{ arr.append(4) };

	int appendValue{ static_cast<int>(numOfElements) };
	for (size_t remElId{ 0 }; remElId < numOfElements; ++remElId)
	{

		arr.remove(remElId);
		EXPECT_EQ(arr.getLen(), numOfElements - 1);

		for (size_t elId{ 0 }; elId < numOfElements; ++elId)
		{

			if (elId < remElId)
				EXPECT_EQ(arr[elId], static_cast<int>(numOfElements + 1 + elId));
			else if  (elId != remElId)
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

TEST(ArrayStaticStable, tsRandomAccess)
{

	using source_t = hfog::Sources::Stack<16_B, hfog::GarbageWriter::Default>;

	static constexpr size_t numOfElements{ 4 };
	fovere::Array::StaticStable<source_t, int, numOfElements> arr;
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

static void createReversed(auto& arr)
{
	auto el0Id{ arr.append(1) };
	auto el1Id{ arr.append(2) };
	auto el2Id{ arr.append(3) };
	auto el3Id{ arr.append(4) };

	arr.remove(3);
	arr.remove(2);
	arr.remove(1);
	arr.remove(0);

	el0Id = arr.append(1);
	el1Id = arr.append(2);
	el2Id = arr.append(3);
	el3Id = arr.append(4);
}

template <typename T>
static void tsIterationRead()
{
	using source_t = hfog::Sources::Stack<16_B, hfog::GarbageWriter::Default>;

	static constexpr size_t numOfElements{ 4 };
	fovere::Array::StaticStable<source_t, T, numOfElements> arr;
	createReversed(arr);

	size_t cnt{ 0 };
	for (size_t id{ 0 }; const auto el : arr)
	{
		++cnt;
		EXPECT_EQ(el, static_cast<T>(++id));
	}
	EXPECT_EQ(cnt, 4);
}

TEST(ArrayStaticStable, tsIteration)
{

	tsIterationRead<int>();
	tsIterationRead<const int>();

	{

		using source_t = hfog::Sources::Stack<16_B, hfog::GarbageWriter::Default>;

		static constexpr size_t numOfElements{ 4 };
		fovere::Array::StaticStable<source_t, int, numOfElements> arr;
		createReversed(arr);

		for (size_t id{ 0 }; auto& el : arr)
		{
			el = 10 + ++id;
		}	

		for (size_t id{ 0 }; const auto el : arr)
		{
			EXPECT_EQ(el, static_cast<int>(10 + ++id));
		}

	}

}