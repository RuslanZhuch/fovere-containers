#include "pch.h"

//import AlgLinear;
import hfog.Algorithms.Stack;
import hfog.Algorithms.Unified;
import hfog.Sources.Local;

import fovere.Array.Static;

using namespace hfog::MemoryUtils::Literals;

static constexpr auto NUM_OF_ELEMENTS{ 3 };
using type_t = int;
using arr_t = fovere::Array::Static<NUM_OF_ELEMENTS, type_t>;

TEST(ArrayStatic, tsAppend)
{

	arr_t arr;
	EXPECT_EQ(arr.getLen(), 0);

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		EXPECT_EQ(arr.append(1), 0);
		EXPECT_EQ(arr.getLen(), 1);
		EXPECT_EQ(arr[0], 1);

		EXPECT_EQ(arr.append(2), 1);
		EXPECT_EQ(arr.getLen(), 2);
		EXPECT_EQ(arr[1], 2);

		EXPECT_EQ(arr.append(3), 2);
		EXPECT_EQ(arr.getLen(), 3);
		EXPECT_EQ(arr[2], 3);

		arr.clear();
		EXPECT_EQ(arr.getLen(), 0);

	}

}

TEST(ArrayStatic, tsRemove)
{

	arr_t arr;
	EXPECT_EQ(arr.getLen(), 0);

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		EXPECT_EQ(arr.append(1), 0);
		EXPECT_EQ(arr.getLen(), 1);
		EXPECT_EQ(arr[0], 1);

		EXPECT_EQ(arr.append(2), 1);;
		EXPECT_EQ(arr.getLen(), 2);
		EXPECT_EQ(arr[1], 2);

		EXPECT_EQ(arr.append(3), 2);;
		EXPECT_EQ(arr.getLen(), 3);
		EXPECT_EQ(arr[2], 3);

		arr.remove(2);
		EXPECT_EQ(arr.getLen(), 2);
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);

		arr.remove(1);
		EXPECT_EQ(arr.getLen(), 1);
		EXPECT_EQ(arr[0], 1);

		arr.remove(0);
		EXPECT_EQ(arr.getLen(), 0);

	}

}

TEST(ArrayStatic, tsRemove2)
{

	arr_t arr;
	EXPECT_EQ(arr.getLen(), 0);

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		EXPECT_EQ(arr.append(1), 0);;
		EXPECT_EQ(arr.getLen(), 1);
		EXPECT_EQ(arr[0], 1);

		EXPECT_EQ(arr.append(2), 1);;
		EXPECT_EQ(arr.getLen(), 2);
		EXPECT_EQ(arr[1], 2);

		EXPECT_EQ(arr.append(3), 2);;
		EXPECT_EQ(arr.getLen(), 3);
		EXPECT_EQ(arr[2], 3);

		arr.remove(1);
		EXPECT_EQ(arr.getLen(), 2);
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 3);

		arr.remove(0);
		EXPECT_EQ(arr.getLen(), 1);
		EXPECT_EQ(arr[0], 3);

		arr.remove(0);
		EXPECT_EQ(arr.getLen(), 0);

	}

}

TEST(ArrayStatic, tsRemoveRange)
{

	fovere::Array::Static<6, type_t> arr;

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		size_t beginId{ 1 };
		size_t endId{ 4 };

		EXPECT_EQ(arr.append(1), 0);
		EXPECT_EQ(arr.append(2), 1);
		EXPECT_EQ(arr.append(3), 2);
		EXPECT_EQ(arr.append(4), 3);
		EXPECT_EQ(arr.append(5), 4);
		EXPECT_EQ(arr.append(6), 5);
		EXPECT_EQ(arr.getLen(), 6);

		arr.remove(beginId, endId);

		EXPECT_EQ(arr.getLen(), 3);
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 5);
		EXPECT_EQ(arr[2], 6);

		arr.remove(0, 3);
		EXPECT_EQ(arr.getLen(), 0);

	}
}

TEST(ArrayStatic, tsInsertion)
{

	fovere::Array::Static<6, type_t> arr;

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		for (size_t insertId{ 0 }; insertId < 5; ++insertId)
		{

			EXPECT_EQ(arr.append(1), 0);
			EXPECT_EQ(arr.append(2), 1);
			EXPECT_EQ(arr.append(3), 2);
			EXPECT_EQ(arr.append(4), 3);
			EXPECT_EQ(arr.append(5), 4);

			EXPECT_TRUE(arr.insert(insertId, -1));

			EXPECT_EQ(arr.getLen(), 6);

			for (size_t id{ 0 }; id < 6; ++id)
			{
				if (id <= insertId)
					EXPECT_EQ(arr[id], id + 1);
				else if (id > insertId + 1)
					EXPECT_EQ(arr[id], id);
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
	arr.append(1);
	arr.append(2);
	arr.append(3);
	arr.append(4);

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
	for (size_t id{ 0 }; auto el: arr)
	{
		++numOfIters;
	}
	EXPECT_EQ(numOfIters, 0);

}