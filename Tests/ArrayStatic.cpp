#include "pch.h"

//import AlgLinear;
import hfog.Algorithms.Stack;
import hfog.Algorithms.Unified;
import hfog.Sources.Local;

import fovere.Array.Static;

using namespace hfog::MemoryUtils::Literals;

static constexpr auto NUM_OF_ELEMENTS{ 4 };
using type_t = int;
using arr_t = fovere::Array::Static<NUM_OF_ELEMENTS, type_t>;

TEST(ArrayStatic, tsAppend)
{

	arr_t arr;
	EXPECT_EQ(arr.getLen(), size_t(0));

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

		EXPECT_EQ(arr.append(4), size_t(3));
		EXPECT_EQ(arr.getLen(), size_t(4));
		EXPECT_EQ(arr[3], 4);

		arr.clear();
		EXPECT_EQ(arr.getLen(), size_t(0));

	}

}

TEST(ArrayStatic, tsRemove)
{

	arr_t arr;
	EXPECT_EQ(arr.getLen(), size_t(0));

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

		EXPECT_EQ(arr.append(4), size_t(3));
		EXPECT_EQ(arr.getLen(), size_t(4));
		EXPECT_EQ(arr[3], 4);

		arr.remove(2);
		EXPECT_EQ(arr.getLen(), size_t(3));
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr[2], 4);

		arr.remove(2);
		EXPECT_EQ(arr.getLen(), size_t(2));
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);

		arr.remove(1);
		EXPECT_EQ(arr.getLen(), size_t(1));
		EXPECT_EQ(arr[0], 1);

		arr.remove(0);
		EXPECT_EQ(arr.getLen(), size_t(0));

	}

}

TEST(ArrayStatic, tsRemove2)
{

	arr_t arr;
	EXPECT_EQ(arr.getLen(), size_t(0));

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

		EXPECT_EQ(arr.append(4), size_t(3));
		EXPECT_EQ(arr.getLen(), size_t(4));
		EXPECT_EQ(arr[3], 4);

		arr.remove(3);
		EXPECT_EQ(arr.getLen(), size_t(3));
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);
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

TEST(ArrayStatic, tsRemoveRange)
{

	fovere::Array::Static<6, type_t> arr;

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

TEST(ArrayStatic, tsInsertion)
{

	fovere::Array::Static<6, type_t> arr;

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

TEST(ArrayStatic, tsForEach)
{

	fovere::Array::Static<4, type_t> arr;
	[[maybe_unused]] const auto a1{ arr.append(1) };
	[[maybe_unused]] const auto a2{ arr.append(2) };
	[[maybe_unused]] const auto a3{ arr.append(3) };
	[[maybe_unused]] const auto a4{ arr.append(4) };

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