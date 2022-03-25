#include "pch.h"

//import AlgLinear;
import AlgStack;
import AlgUnified;
import SourceStack;

import fovere.Adapter;

using namespace hfog::MemoryUtils::Literals;

//TEST(Adapters, tsLinearAdapter)
//{
//
//	using source_t = hfog::Sources::Stack<12_B, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
//	using allocator_t = hfog::Algorithms::Linear<source_t, 4_B>;
//
//	fovere::Adapter<allocator_t, int> adapter;
//	EXPECT_EQ(adapter.getLen(), 0);
//
//	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
//	{
//		adapter.append(1);
//		EXPECT_EQ(adapter.getLen(), 1);
//		EXPECT_EQ(adapter[0], 1);
//
//		adapter.append(2);
//		EXPECT_EQ(adapter.getLen(), 2);
//		EXPECT_EQ(adapter[1], 2);
//
//		adapter.append(3);
//		EXPECT_EQ(adapter.getLen(), 3);
//		EXPECT_EQ(adapter[2], 3);
//
//		adapter.clear();
//		EXPECT_EQ(adapter.getLen(), 0);
//	}
//
//	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
//	{
//
//		adapter.append(1);
//		EXPECT_EQ(adapter.getLen(), 1);
//		EXPECT_EQ(adapter[0], 1);
//
//		adapter.append(2);
//		EXPECT_EQ(adapter.getLen(), 2);
//		EXPECT_EQ(adapter[1], 2);
//
//		adapter.append(3);
//		EXPECT_EQ(adapter.getLen(), 3);
//		EXPECT_EQ(adapter[2], 3);
//
//		adapter.remove(0);
//		EXPECT_EQ(adapter.getLen(), 3);
//		adapter.remove(1);
//		EXPECT_EQ(adapter.getLen(), 3);
//
//		adapter.remove(2);
//		EXPECT_EQ(adapter.getLen(), 2);
//		EXPECT_EQ(adapter[0], 1);
//		EXPECT_EQ(adapter[1], 2);
//
//		adapter.remove(0);
//		EXPECT_EQ(adapter.getLen(), 2);
//		
//		adapter.remove(1);
//		EXPECT_EQ(adapter.getLen(), 1);
//		EXPECT_EQ(adapter[0], 1);
//
//		adapter.remove(1);
//		EXPECT_EQ(adapter.getLen(), 1);
//		EXPECT_EQ(adapter[0], 1);
//
//		adapter.remove(0);
//		EXPECT_EQ(adapter.getLen(), 0);
//
//	}
//
//}

TEST(Adapters, tsStackAdapter)
{

	using source_t = hfog::Sources::Stack<12_B, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
	using allocator_t = hfog::Algorithms::Stack<source_t, 4_B>;

	fovere::Adapter<allocator_t, int> adapter;
	EXPECT_EQ(adapter.getLen(), 0);

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		EXPECT_EQ(adapter.append(1), 0);
		EXPECT_EQ(adapter.getLen(), 1);
		EXPECT_EQ(adapter[0], 1);

		EXPECT_EQ(adapter.append(2), 1);
		EXPECT_EQ(adapter.getLen(), 2);
		EXPECT_EQ(adapter[1], 2);

		EXPECT_EQ(adapter.append(3), 2);
		EXPECT_EQ(adapter.getLen(), 3);
		EXPECT_EQ(adapter[2], 3);

		adapter.clear();
		EXPECT_EQ(adapter.getLen(), 0);

	}

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		EXPECT_EQ(adapter.append(1), 0);;
		EXPECT_EQ(adapter.getLen(), 1);
		EXPECT_EQ(adapter[0], 1);

		EXPECT_EQ(adapter.append(2), 1);;
		EXPECT_EQ(adapter.getLen(), 2);
		EXPECT_EQ(adapter[1], 2);

		EXPECT_EQ(adapter.append(3), 2);;
		EXPECT_EQ(adapter.getLen(), 3);
		EXPECT_EQ(adapter[2], 3);

		adapter.remove(2);
		EXPECT_EQ(adapter.getLen(), 2);
		EXPECT_EQ(adapter[0], 1);
		EXPECT_EQ(adapter[1], 2);

		adapter.remove(1);
		EXPECT_EQ(adapter.getLen(), 1);
		EXPECT_EQ(adapter[0], 1);

		adapter.remove(0);
		EXPECT_EQ(adapter.getLen(), 0);

	}

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		EXPECT_EQ(adapter.append(1), 0);;
		EXPECT_EQ(adapter.getLen(), 1);
		EXPECT_EQ(adapter[0], 1);

		EXPECT_EQ(adapter.append(2), 1);;
		EXPECT_EQ(adapter.getLen(), 2);
		EXPECT_EQ(adapter[1], 2);

		EXPECT_EQ(adapter.append(3), 2);;
		EXPECT_EQ(adapter.getLen(), 3);
		EXPECT_EQ(adapter[2], 3);

		adapter.remove(1);
		EXPECT_EQ(adapter.getLen(), 2);
		EXPECT_EQ(adapter[0], 1);
		EXPECT_EQ(adapter[1], 3);

		adapter.remove(0);
		EXPECT_EQ(adapter.getLen(), 1);
		EXPECT_EQ(adapter[0], 3);

		adapter.remove(0);
		EXPECT_EQ(adapter.getLen(), 0);

	}

}

TEST(Adapters, tsStackAdapterRemoveRange)
{

	using source_t = hfog::Sources::Stack<24_B, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
	using allocator_t = hfog::Algorithms::Stack<source_t, 4_B>;

	fovere::Adapter<allocator_t, int> adapter;

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		size_t beginId{ 1 };
		size_t endId{ 4 };

		EXPECT_EQ(adapter.append(1), 0);
		EXPECT_EQ(adapter.append(2), 1);
		EXPECT_EQ(adapter.append(3), 2);
		EXPECT_EQ(adapter.append(4), 3);
		EXPECT_EQ(adapter.append(5), 4);
		EXPECT_EQ(adapter.append(6), 5);
		EXPECT_EQ(adapter.getLen(), 6);

		adapter.remove(beginId, endId);

		EXPECT_EQ(adapter.getLen(), 3);
		EXPECT_EQ(adapter[0], 1);
		EXPECT_EQ(adapter[1], 5);
		EXPECT_EQ(adapter[2], 6);

		adapter.remove(0, 3);
		EXPECT_EQ(adapter.getLen(), 0);

	}
}

TEST(Adapters, tsStackAdapterInsertion)
{

	using source_t = hfog::Sources::Stack<24_B, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
	using allocator_t = hfog::Algorithms::Stack<source_t, 4_B>;

	fovere::Adapter<allocator_t, int> adapter;

	for (size_t itersLeft{ 2 }; itersLeft > 0; --itersLeft)
	{

		for (size_t insertId{ 0 }; insertId < 5; ++insertId)
		{

			EXPECT_EQ(adapter.append(1), 0);
			EXPECT_EQ(adapter.append(2), 1);
			EXPECT_EQ(adapter.append(3), 2);
			EXPECT_EQ(adapter.append(4), 3);
			EXPECT_EQ(adapter.append(5), 4);

			adapter.insert(insertId, -1);

			EXPECT_EQ(adapter.getLen(), 6);

			for (size_t id{ 0 }; id < 6; ++id)
			{
				if (id <= insertId)
					EXPECT_EQ(adapter[id], id + 1);
				else if (id > insertId + 1)
					EXPECT_EQ(adapter[id], id);
				else
					EXPECT_EQ(adapter[id], -1);
			}

			adapter.clear();

		}

	}

}