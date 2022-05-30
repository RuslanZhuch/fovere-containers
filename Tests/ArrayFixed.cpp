#include "pch.h"

import fovere.Array.Fixed;
import fovere.Core;

static constexpr auto NUM_OF_ELEMETNS{ 5 };
using type_t = int;
using arr_t = fovere::Array::Fixed<type_t, NUM_OF_ELEMETNS>;

static_assert(fovere::Core::CtContainerContinuous<fovere::Array::Fixed<type_t, NUM_OF_ELEMETNS>>);

TEST(FixedArray, tsConstructor)
{
	
	static constexpr auto VALUE{ 42 };

	arr_t arr(VALUE);

	EXPECT_EQ(arr[0], VALUE);
	EXPECT_EQ(arr[1], VALUE);
	EXPECT_EQ(arr[2], VALUE);
	EXPECT_EQ(arr[3], VALUE);
	EXPECT_EQ(arr[4], VALUE);

}

TEST(FixedArray, tsBasicOps)
{
	
	arr_t arr;
	EXPECT_EQ(arr.getLen(), NUM_OF_ELEMETNS);
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	arr[3] = 4;
	arr[4] = 5;

	EXPECT_EQ(arr[0], 1);
	EXPECT_EQ(arr[1], 2);
	EXPECT_EQ(arr[2], 3);
	EXPECT_EQ(arr[3], 4);
	EXPECT_EQ(arr[4], 5);

}

TEST(FixedArray, tsForEach)
{

	arr_t arr;
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	arr[3] = 4;
	arr[4] = 5;

	type_t expectedEl{ 1 };
	for (auto el : arr)
	{
		EXPECT_EQ(el, expectedEl++);
	}

}