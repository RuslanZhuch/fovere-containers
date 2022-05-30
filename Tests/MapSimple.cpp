#include "pch.h"

#include "MapTestCases.h"

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


template <typename Alloc, typename Func>
void testWithLocalBuffer(Func func)
{

	Alloc alloc;

	fovere::Map::Simple<int, size_t, Alloc> map(&alloc);

	func(std::move(map));

}

template <typename Alloc, size_t numOfBytes, typename Func>
void testWithExternalBuffer(Func func)
{

	byte_t extBuffer[numOfBytes];
	Alloc alloc(hfog::MemoryBlock(extBuffer, sizeof(extBuffer)));

	fovere::Map::Simple<int, size_t, Alloc> map(&alloc);

	func(std::move(map));

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

TEST(MapSimple, tsCreation)
{

	testFull<MapTestCases::MAX_BYTES, MapTestCases::NUM_OF_CHUNCKS, MapTestCases::EXT_BUFFER_SIZE>([](auto&& map) {MapTestCases::tsImplCreation(map); });

}

TEST(MapSimple, tsRemove)
{

	testFull<MapTestCases::MAX_BYTES, MapTestCases::NUM_OF_CHUNCKS, MapTestCases::EXT_BUFFER_SIZE>([](auto&& map) {MapTestCases::tsImplRemove(map); });

}

TEST(MapSimple, tsFind)
{

	testFull<MapTestCases::MAX_BYTES, MapTestCases::NUM_OF_CHUNCKS, MapTestCases::EXT_BUFFER_SIZE>([](auto&& map) {MapTestCases::tsImplFind(map); });

}

TEST(MapSimple, tsForEachKey)
{

	testFull<MapTestCases::MAX_BYTES, MapTestCases::NUM_OF_CHUNCKS, MapTestCases::EXT_BUFFER_SIZE>([](auto&& map) {MapTestCases::tsImplForEachKey(map); });

}

TEST(MapSimple, tsForEachItem)
{

	testFull<MapTestCases::MAX_BYTES, MapTestCases::NUM_OF_CHUNCKS, MapTestCases::EXT_BUFFER_SIZE>([](auto&& map) {MapTestCases::tsImplForEachItem(map); });

}

TEST(MapSimple, tsForEach)
{

	testFull<MapTestCases::MAX_BYTES, MapTestCases::NUM_OF_CHUNCKS, MapTestCases::EXT_BUFFER_SIZE>([](auto&& map) {MapTestCases::tsImplForEach(map); });

}