#include "pch.h"

//import AlgLinear;
import hfog.Algorithms.Stack;
import hfog.Algorithms.Unified;
import hfog.Sources.Stack;

import fovere.Array.Static;

using namespace hfog::MemoryUtils::Literals;

//TEST(ArrayStatic, tsLinearAdapter)
//{
//
//	using source_t = hfog::Sources::Stack<12_B, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
//	using allocator_t = hfog::Algorithms::Linear<source_t, 4_B>;
//
//	fovere::Array::Static<allocator_t, int> arr;
//	EXPECT_EQ(arr.getLen(), 0);
//
//	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
//	{
//		arr.append(1);
//		EXPECT_EQ(arr.getLen(), 1);
//		EXPECT_EQ(arr[0], 1);
//
//		arr.append(2);
//		EXPECT_EQ(arr.getLen(), 2);
//		EXPECT_EQ(arr[1], 2);
//
//		arr.append(3);
//		EXPECT_EQ(arr.getLen(), 3);
//		EXPECT_EQ(arr[2], 3);
//
//		arr.clear();
//		EXPECT_EQ(arr.getLen(), 0);
//	}
//
//	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
//	{
//
//		arr.append(1);
//		EXPECT_EQ(arr.getLen(), 1);
//		EXPECT_EQ(arr[0], 1);
//
//		arr.append(2);
//		EXPECT_EQ(arr.getLen(), 2);
//		EXPECT_EQ(arr[1], 2);
//
//		arr.append(3);
//		EXPECT_EQ(arr.getLen(), 3);
//		EXPECT_EQ(arr[2], 3);
//
//		arr.remove(0);
//		EXPECT_EQ(arr.getLen(), 3);
//		arr.remove(1);
//		EXPECT_EQ(arr.getLen(), 3);
//
//		arr.remove(2);
//		EXPECT_EQ(arr.getLen(), 2);
//		EXPECT_EQ(arr[0], 1);
//		EXPECT_EQ(arr[1], 2);
//
//		arr.remove(0);
//		EXPECT_EQ(arr.getLen(), 2);
//		
//		arr.remove(1);
//		EXPECT_EQ(arr.getLen(), 1);
//		EXPECT_EQ(arr[0], 1);
//
//		arr.remove(1);
//		EXPECT_EQ(arr.getLen(), 1);
//		EXPECT_EQ(arr[0], 1);
//
//		arr.remove(0);
//		EXPECT_EQ(arr.getLen(), 0);
//
//	}
//
//}

TEST(ArrayStatic, tsStackAdapter)
{

	fovere::Array::Static<3, int> arr;
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

TEST(ArrayStatic, tsStackAdapterRemoveRange)
{

	fovere::Array::Static<6, int> arr;

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

TEST(ArrayStatic, tsStackAdapterInsertion)
{

	fovere::Array::Static<6, int> arr;

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