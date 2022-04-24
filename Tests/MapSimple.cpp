#include "pch.h"
#include <array>
import hfog.Alloc;
import fovere.Map.Simple;

using namespace hfog::MemoryUtils::Literals;

TEST(MapSimple, tsCreation)
{

	byte_t extBuffer[512_B];

	using allocator_t = hfog::Alloc::IslandsExt<4_B, 128_B, 4>;

	allocator_t alloc(hfog::MemoryBlock(extBuffer, sizeof(extBuffer)));

	fovere::Map::Simple<int, size_t, allocator_t> map(&alloc);

	EXPECT_EQ(map.getLen(), 0);

	for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
	{

		map[0] = 101;
		EXPECT_EQ(map.getLen(), 1);
		EXPECT_EQ(map[0], 101);
		map[1] = 102;
		EXPECT_EQ(map.getLen(), 2);
		EXPECT_EQ(map[0], 101);
		EXPECT_EQ(map[1], 102);

		map.insert(2, 103);
		EXPECT_EQ(map.getLen(), 3);
		EXPECT_EQ(map[0], 101);
		EXPECT_EQ(map[1], 102);
		EXPECT_EQ(map[2], 103);

		map[1] += 100;
		EXPECT_EQ(map.getLen(), 3);
		EXPECT_EQ(map[0], 101);
		EXPECT_EQ(map[1], 202);
		EXPECT_EQ(map[2], 103);

		map.clear();

	}

}

TEST(MapSimple, tsRemove)
{

	byte_t extBuffer[512_B];

//	using allocator_t = hfog::Alloc::UnifiedExt<128_B, 384_B, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
	using allocator_t = hfog::Alloc::IslandsExt<4_B, 128_B, 4, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;

	allocator_t alloc(hfog::MemoryBlock(extBuffer,sizeof(extBuffer)));

	fovere::Map::Simple<int, size_t, allocator_t> map(&alloc);

	EXPECT_EQ(map.getLen(), 0);

	for (size_t numOfIters{ 0 }; numOfIters < 1; ++numOfIters)
	{

		map[0] = 101;
		EXPECT_EQ(map.getLen(), 1);
		EXPECT_EQ(map[0], 101);
		map[1] = 102;
		EXPECT_EQ(map.getLen(), 2);
		EXPECT_EQ(map[0], 101);
		EXPECT_EQ(map[1], 102);

		map.insert(2, 103);
		EXPECT_EQ(map.getLen(), 3);
		EXPECT_EQ(map[0], 101);
		EXPECT_EQ(map[1], 102);
		EXPECT_EQ(map[2], 103);

		map.remove(1);
		EXPECT_EQ(map.getLen(), 2);
		EXPECT_EQ(map[0], 101);
		EXPECT_EQ(map[2], 103);

		map.remove(0);
		EXPECT_EQ(map.getLen(), 1);
		EXPECT_EQ(map[2], 103);

		map.remove(2);
		EXPECT_EQ(map.getLen(), 0);

		map.remove(2);
		map.remove(1);
		map.remove(0);

		map.clear();


		map[0] = 101;
		EXPECT_EQ(map.getLen(), 1);
		EXPECT_EQ(map[0], 101);
		map[1] = 102;
		EXPECT_EQ(map.getLen(), 2);
		EXPECT_EQ(map[0], 101);
		EXPECT_EQ(map[1], 102);

		map.insert(2, 103);
		EXPECT_EQ(map.getLen(), 3);
		EXPECT_EQ(map[0], 101);
		EXPECT_EQ(map[1], 102);
		EXPECT_EQ(map[2], 103);

		map.clear();

	}

}

TEST(MapSimple, tsFind)
{

	byte_t extBuffer[512_B];

	//	using allocator_t = hfog::Alloc::UnifiedExt<128_B, 384_B, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;
	using allocator_t = hfog::Alloc::IslandsExt<4_B, 128_B, 4, hfog::GarbageWriter::ByteWriter<0xFA, 0xAF>>;

	allocator_t alloc(hfog::MemoryBlock(extBuffer, sizeof(extBuffer)));

	fovere::Map::Simple<int, size_t, allocator_t> map(&alloc);

	EXPECT_EQ(map.getLen(), 0);

	for (size_t numOfIters{ 0 }; numOfIters < 1; ++numOfIters)
	{

		EXPECT_FALSE(map.find(0).has_value());

		map[0] = 101;
		EXPECT_EQ(map.find(0).value(), 101);
		EXPECT_FALSE(map.find(1).has_value());

		map[1] = 102;
		EXPECT_EQ(map.find(0).value(), 101);
		EXPECT_EQ(map.find(1).value(), 102);

		map.remove(0);
		EXPECT_FALSE(map.find(0).has_value());
		EXPECT_EQ(map.find(1).value(), 102);

		map.clear();
		EXPECT_FALSE(map.find(0).has_value());
		EXPECT_FALSE(map.find(1).has_value());

	}

}