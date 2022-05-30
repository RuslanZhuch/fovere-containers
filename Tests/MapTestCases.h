#pragma once
#include "pch.h"

import hfog.Alloc;

namespace MapTestCases
{

	using namespace hfog::MemoryUtils::Literals;

	static constexpr auto MAX_BYTES{ 128_B };
	static constexpr auto NUM_OF_CHUNCKS{ 4 };
	static constexpr auto EXT_BUFFER_SIZE{ 512_B };

	void tsImplCreation(auto&& map)
	{

		EXPECT_EQ(map.getLen(), size_t(0));

		for (size_t numOfIters{ 0 }; numOfIters < 2; ++numOfIters)
		{

			map[0] = 101;
			EXPECT_EQ(map.getLen(), size_t(1));
			EXPECT_EQ(map[0], size_t(101));
			map[1] = 102;
			EXPECT_EQ(map.getLen(), size_t(2));
			EXPECT_EQ(map[0], size_t(101));
			EXPECT_EQ(map[1], size_t(102));

			map.insert(2, 103);
			EXPECT_EQ(map.getLen(), size_t(3));
			EXPECT_EQ(map[0], size_t(101));
			EXPECT_EQ(map[1], size_t(102));
			EXPECT_EQ(map[2], size_t(103));

			map[1] += 100;
			EXPECT_EQ(map.getLen(), size_t(3));
			EXPECT_EQ(map[0], size_t(101));
			EXPECT_EQ(map[1], size_t(202));
			EXPECT_EQ(map[2], size_t(103));

			map.clear();

		}

	}

	void tsImplRemove(auto&& map)
	{

		for (size_t numOfIters{ 0 }; numOfIters < 1; ++numOfIters)
		{

			map[0] = 101;
			EXPECT_EQ(map.getLen(), size_t(1));
			EXPECT_EQ(map[0], size_t(101));
			map[1] = 102;
			EXPECT_EQ(map.getLen(), size_t(2));
			EXPECT_EQ(map[0], size_t(101));
			EXPECT_EQ(map[1], size_t(102));

			map.insert(2, 103);
			EXPECT_EQ(map.getLen(), size_t(3));
			EXPECT_EQ(map[0], size_t(101));
			EXPECT_EQ(map[1], size_t(102));
			EXPECT_EQ(map[2], size_t(103));

			map.remove(1);
			EXPECT_EQ(map.getLen(), size_t(2));
			EXPECT_EQ(map[0], size_t(101));
			EXPECT_EQ(map[2], size_t(103));

			map.remove(0);
			EXPECT_EQ(map.getLen(), size_t(1));
			EXPECT_EQ(map[2], size_t(103));

			map.remove(2);
			EXPECT_EQ(map.getLen(), size_t(0));

			map.remove(2);
			map.remove(1);
			map.remove(0);

			map.clear();

			map[0] = 101;
			EXPECT_EQ(map.getLen(), size_t(1));
			EXPECT_EQ(map[0], size_t(101));
			map[1] = 102;
			EXPECT_EQ(map.getLen(), size_t(2));
			EXPECT_EQ(map[0], size_t(101));
			EXPECT_EQ(map[1], size_t(102));

			map.insert(2, 103);
			EXPECT_EQ(map.getLen(), size_t(3));
			EXPECT_EQ(map[0], size_t(101));
			EXPECT_EQ(map[1], size_t(102));
			EXPECT_EQ(map[2], size_t(103));

			map.clear();

		}

	}

	void tsImplFind(auto&& map)
	{

		for (size_t numOfIters{ 0 }; numOfIters < 1; ++numOfIters)
		{

			EXPECT_FALSE(map.find(0).has_value());

			map[0] = 101;
			EXPECT_EQ(map.find(0).value(), size_t(101));
			EXPECT_FALSE(map.find(1).has_value());

			map[1] = 102;
			EXPECT_EQ(map.find(0).value(), size_t(101));
			EXPECT_EQ(map.find(1).value(), size_t(102));

			map.remove(0);
			EXPECT_FALSE(map.find(0).has_value());
			EXPECT_EQ(map.find(1).value(), size_t(102));

			map.clear();
			EXPECT_FALSE(map.find(0).has_value());
			EXPECT_FALSE(map.find(1).has_value());

		}

	}


	void tsImplForEachKey(auto&& map)
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

	void tsImplForEachItem(auto&& map)
	{

		map[10] = 101;
		map[20] = 102;
		map[30] = 103;

		const auto& items{ map.getItems() };
		size_t expectedItem = 101;

		for (auto item : items)
		{
			EXPECT_EQ(item, expectedItem);
			++expectedItem;
		}

		EXPECT_EQ(expectedItem, size_t(104));

	}

	void tsImplForEach(auto&& map)
	{

		map[10] = 101;
		map[20] = 102;
		map[30] = 103;

		int expectedKey = 10;
		size_t expectedItem = 101;

		for (const auto& [key, item] : map)
		{
			EXPECT_EQ(*key, expectedKey);
			EXPECT_EQ(*item, expectedItem);
			expectedKey += 10;
			++expectedItem;
		}

		EXPECT_EQ(expectedKey, 40);
		EXPECT_EQ(expectedItem, size_t(104));

	}

};
