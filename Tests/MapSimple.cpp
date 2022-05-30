#include "pch.h"
#include <array>
import hfog.Alloc;
import fovere.Map.Simple;

using namespace hfog::MemoryUtils::Literals;

template<mem_t alignment, mem_t maxAllocBytes, size_t numOfChunks>
using allocIslandsExt_t = hfog::Alloc::IslandsExt<alignment, maxAllocBytes, numOfChunks, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;

template<mem_t alignment, mem_t maxAllocBytes, size_t numOfChunks>
using allocIslands_t = hfog::Alloc::Islands<alignment, maxAllocBytes, numOfChunks, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;


template<mem_t alignment, size_t numOfChunks>
using allocPoolExt_t = hfog::Alloc::PoolExt<alignment, numOfChunks, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;

template<mem_t alignment, size_t numOfChunks>
using allocPool_t = hfog::Alloc::Pool<alignment, numOfChunks, hfog::GarbageWriter::ByteWriter<SET_BYTES, CLEAR_BYTES>>;

static constexpr auto MAX_BYTES{ 128_B };
static constexpr auto NUM_OF_CHUNCKS{ 4 };
static constexpr auto EXT_BUFFER_SIZE{ 512_B };

template <typename Alloc, typename Func>
void testWithLocalBuffer(Func func)
{

	Alloc alloc;

	fovere::Map::Simple<int, size_t, Alloc> arr(&alloc);

	func(arr);

}

template <typename Alloc, size_t numOfBytes, typename Func>
void testWithExternalBuffer(Func func)
{

	byte_t extBuffer[numOfBytes];
	Alloc alloc(hfog::MemoryBlock(extBuffer, sizeof(extBuffer)));

	fovere::Map::Simple<int, size_t, Alloc> arr(&alloc);

	func(arr);

}

template <mem_t maxAllocBytes, size_t numOfChunks, size_t numOfExtBytes, typename Func>
void testFull(Func func)
{

	constexpr auto minAlignment{ 4_B };

	testWithLocalBuffer<allocIslands_t<minAlignment, maxAllocBytes, numOfChunks>>(func);
	testWithLocalBuffer<allocPool_t<maxAllocBytes, numOfChunks>>(func);

	testWithExternalBuffer<allocIslandsExt_t<minAlignment, maxAllocBytes, numOfChunks>, numOfExtBytes>(func);
	testWithExternalBuffer<allocPoolExt_t<maxAllocBytes, numOfChunks>, numOfExtBytes>(func);

}

void tsImplCreation(auto map)
{

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

TEST(MapSimple, tsCreation)
{

	testFull<MAX_BYTES, NUM_OF_CHUNCKS, EXT_BUFFER_SIZE>([](auto map) {tsImplCreation(map); });

}

void tsImplRemove(auto map)
{

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

TEST(MapSimple, tsRemove)
{

	testFull<MAX_BYTES, NUM_OF_CHUNCKS, EXT_BUFFER_SIZE>([](auto map) {tsImplRemove(map); });

}

void tsImplFind(auto map)
{

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

TEST(MapSimple, tsFind)
{

	testFull<MAX_BYTES, NUM_OF_CHUNCKS, EXT_BUFFER_SIZE>([](auto map) {tsImplFind(map); });

}

void tsImplForEachKey(auto map)
{

	map[10] = 101;
	map[20] = 102;
	map[30] = 103;

	const auto& keys{ map.getKeys() };
	int expectedKey = 10;

	for (auto key : keys)
	{
		EXPECT_EQ(key, expectedKey);
		expectedKey += 10;
	}

	EXPECT_EQ(expectedKey, 40);

}

TEST(MapSimple, tsForEachKey)
{

	testFull<MAX_BYTES, NUM_OF_CHUNCKS, EXT_BUFFER_SIZE>([](auto map) {tsImplForEachKey(map); });

}

void tsImplForEachItem(auto map)
{

	map[10] = 101;
	map[20] = 102;
	map[30] = 103;

	const auto& items{ map.getItems() };
	int expectedItem = 101;

	for (auto item : items)
	{
		EXPECT_EQ(item, expectedItem);
		++expectedItem;
	}

	EXPECT_EQ(expectedItem, 104);

}

TEST(MapSimple, tsForEachItem)
{

	testFull<MAX_BYTES, NUM_OF_CHUNCKS, EXT_BUFFER_SIZE>([](auto map) {tsImplForEachItem(map); });

}

void tsImplForEach(auto map)
{

	map[10] = 101;
	map[20] = 102;
	map[30] = 103;

	int expectedKey = 10;
	int expectedItem = 101;

	for (const auto& [key, item] : map)
	{
		EXPECT_EQ(*key, expectedKey);
		EXPECT_EQ(*item, expectedItem);
		expectedKey += 10;
		++expectedItem;
	}

	EXPECT_EQ(expectedKey, 40);
	EXPECT_EQ(expectedItem, 104);

}

TEST(MapSimple, tsForEach)
{

	testFull<MAX_BYTES, NUM_OF_CHUNCKS, EXT_BUFFER_SIZE>([](auto map) {tsImplForEach(map); });

}