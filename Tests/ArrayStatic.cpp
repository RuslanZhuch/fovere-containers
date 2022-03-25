#include "pch.h"

import fovere.Array.Static;

TEST(StaticArray, tsCreation)
{

	{
		fovere::Array::Static<int, 5> arr;
		EXPECT_EQ(arr.getLen(), 0);

		arr[0] = 1;
		EXPECT_EQ(arr.getLen(), 1);

		arr[1] = 2;
		EXPECT_EQ(arr.getLen(), 2);

		arr[4] = 5;
		EXPECT_EQ(arr.getLen(), 5);

		arr[2] = 3;
		EXPECT_EQ(arr.getLen(), 5);
		arr[3] = 4;
		EXPECT_EQ(arr.getLen(), 5);

		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr[2], 3);
		EXPECT_EQ(arr[3], 4);
		EXPECT_EQ(arr[4], 5);
	}

	{
		fovere::Array::Static<int, 5> arr(3);
		EXPECT_EQ(arr.getLen(), 3);

		arr[0] = 1;
		EXPECT_EQ(arr.getLen(), 3);

		arr[1] = 2;
		EXPECT_EQ(arr.getLen(), 3);

		arr[4] = 5;
		EXPECT_EQ(arr.getLen(), 5);

		arr[2] = 3;
		EXPECT_EQ(arr.getLen(), 5);
		arr[3] = 4;
		EXPECT_EQ(arr.getLen(), 5);

		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr[2], 3);
		EXPECT_EQ(arr[3], 4);
		EXPECT_EQ(arr[4], 5);
	}

	{
		fovere::Array::Static<int, 5> arr(3, -1);
		EXPECT_EQ(arr.getLen(), 3);

		EXPECT_EQ(arr[0], -1);
		EXPECT_EQ(arr[1], -1);
		EXPECT_EQ(arr[2], -1);

		arr[4] = 5;
		EXPECT_EQ(arr.getLen(), 5);
		EXPECT_EQ(arr[4], 5);

		arr[3] = 4;
		EXPECT_EQ(arr.getLen(), 5);

		EXPECT_EQ(arr[0], -1);
		EXPECT_EQ(arr[1], -1);
		EXPECT_EQ(arr[2], -1);
		EXPECT_EQ(arr[3], 4);
		EXPECT_EQ(arr[4], 5);

	}

}

TEST(StaticArray, tsInsertion)
{

	{

		fovere::Array::Static<int, 5> arr;

		EXPECT_EQ(arr.insert(0, 2), fovere::invalidIndex);

		arr[0] = 1;
		EXPECT_EQ(arr.insert(0, 2), 1);
		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);

		EXPECT_EQ(arr.insert(2, 3), fovere::invalidIndex);

		EXPECT_EQ(arr.insert(1, 3), 2);

		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr[2], 3);

		EXPECT_EQ(arr.insert(2, 4), 3);

		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr[2], 3);
		EXPECT_EQ(arr[3], 4);

		EXPECT_EQ(arr.insert(3, 5), 4);

		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr[2], 3);
		EXPECT_EQ(arr[3], 4);
		EXPECT_EQ(arr[4], 5);

		EXPECT_EQ(arr.insert(4, 6), fovere::invalidIndex);

		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr[2], 3);
		EXPECT_EQ(arr[3], 4);
		EXPECT_EQ(arr[4], 5);

	}

	const auto testInsert = [](size_t numOfElements)
	{

		for (size_t iterId{ 0 }; iterId < numOfElements; ++iterId)
		{

			fovere::Array::Static<int, 5> arr(numOfElements);
			for (size_t elId{ 0 }; elId < 5; ++elId)
			{
				arr[elId] = static_cast<int>(elId + 1);
			}

			const auto insertedId{ arr.insert(iterId, 100) };

			int offset{ 0 };
			for (size_t elId{ 0 }; elId < arr.getLen(); ++elId)
			{
				if (elId == insertedId)
				{
					EXPECT_EQ(arr[elId], -1);
					++offset;
				}
				else
					EXPECT_EQ(arr[elId], elId + 1 - offset);
			}

			EXPECT_EQ(arr.getLen(), 5);

		}

	};

	testInsert(4);
	testInsert(5);

}

TEST(StaticArray, tsRemove)
{

	const auto fillArray = [](auto& arr, size_t numOfElements)
	{
		for (size_t elId{ 0 }; elId < numOfElements; ++elId)
		{
			arr[elId] = static_cast<int>(elId + 1);
		}
	};

	{

		fovere::Array::Static<int, 5> arr;
		fillArray(arr, 5);

		arr.remove(3);

		EXPECT_EQ(arr[0], 1);
		EXPECT_EQ(arr[1], 2);
		EXPECT_EQ(arr[2], 3);
		EXPECT_EQ(arr[3], 5);
		EXPECT_EQ(arr.getLen(), 4);

	}

}