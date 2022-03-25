#include "pch.h"

import fovere.Array.Fixed;
import fovere.Core;

TEST(FixedArray, tsCreation)
{
	
	static_assert(fovere::Core::CtContainerContinuous<fovere::Array::Fixed<int, 5>>);

	{

		fovere::Array::Fixed<int, 5> arr;
		EXPECT_EQ(arr.getLen(), 5);
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

	{
		fovere::Array::Fixed<int, 5> arr(42);

		EXPECT_EQ(arr[0], 42);
		EXPECT_EQ(arr[1], 42);
		EXPECT_EQ(arr[2], 42);
		EXPECT_EQ(arr[3], 42);
		EXPECT_EQ(arr[4], 42);
	}

}