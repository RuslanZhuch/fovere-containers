#include "pch.h"
#include "MapTestCases.h"

import hfog.Algorithms.Stack;
import hfog.Algorithms.Unified;
import hfog.Sources.Local;
import hfog.Alloc;

import fovere.Adapter.Static;

import fovere.Map.Simple;

using namespace hfog::MemoryUtils::Literals;

static constexpr auto NUM_OF_ELEMENTS{ 4 };
using typeKey_t = int;
using typeVal_t = size_t;

template <typename Container, typename Func>
void testWithLocalBuffer(Func func)
{

	fovere::Adapter::Static<Container> arr;

	func(std::move(arr));

}

template <mem_t maxAllocBytes, size_t numOfChunks, bool universal = true, typename Func>
void testFull(Func func)
{

	testWithLocalBuffer<fovere::Map::Simple<typeKey_t, typeVal_t, hfog::Algorithms::Pool<hfog::Sources::Local<maxAllocBytes * numOfChunks>, maxAllocBytes, numOfChunks>>>(func);

}

TEST(MapStatic, tsCreation)
{

	testFull<MapTestCases::MAX_BYTES, MapTestCases::NUM_OF_CHUNCKS>([](auto&& map) {MapTestCases::tsImplCreation(map); });

}

TEST(MapStatic, tsRemove)
{

	testFull<MapTestCases::MAX_BYTES, MapTestCases::NUM_OF_CHUNCKS>([](auto&& map) {MapTestCases::tsImplRemove(map); });

}

TEST(MapStatic, tsFind)
{

	testFull<MapTestCases::MAX_BYTES, MapTestCases::NUM_OF_CHUNCKS>([](auto&& map) {MapTestCases::tsImplFind(map); });

}

TEST(MapStatic, tsForEachKey)
{

	testFull<MapTestCases::MAX_BYTES, MapTestCases::NUM_OF_CHUNCKS>([](auto&& map) {MapTestCases::tsImplForEachKey(map); });

}

TEST(MapStatic, tsForEachItem)
{

	testFull<MapTestCases::MAX_BYTES, MapTestCases::NUM_OF_CHUNCKS>([](auto&& map) {MapTestCases::tsImplForEachItem(map); });

}

TEST(MapStatic, tsForEach)
{

	testFull<MapTestCases::MAX_BYTES, MapTestCases::NUM_OF_CHUNCKS>([](auto&& map) {MapTestCases::tsImplForEach(map); });

}
