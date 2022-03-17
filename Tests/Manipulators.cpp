#include "pch.h"

import fovere.Len;
import fovere.Get;
import fovere.Insert;
import fovere.Contain;
import fovere.Count;
import fovere.Find;
import fovere.Fill;
import fovere.Accumulate;

template <typename T, size_t numOfElements>
class TestArray
{
public:
	using TVal = typename T;

public:
	[[nodiscard]] constexpr auto getLen() const noexcept
	{
		return numOfElements;
	}

	[[nodiscard]] constexpr T& operator[](auto id) noexcept
	{
		return this->data[id];
	}

	[[nodiscard]] constexpr T operator[](auto id) const noexcept
	{
		return this->data[id];
	}

private:
	T data[numOfElements];
};

TEST(Manipulators, tsLen)
{
	{
		TestArray<int, 5> arr;
		EXPECT_EQ(fovere::len(arr), 5);
	}
	{
		TestArray<long long, 5> arr;
		EXPECT_EQ(fovere::len(arr), 5);
	}
	{
		TestArray<int, 500> arr;
		EXPECT_EQ(fovere::len(arr), 500);
	}
}

TEST(Manipulators, tsGet)
{
	{
		TestArray<int, 5> arr;
		arr[0] = 1;
		arr[1] = 2;
		arr[2] = 3;
		arr[3] = 4;
		arr[4] = 5;

		EXPECT_EQ(fovere::get(arr, 0), 1);
		EXPECT_EQ(fovere::get(arr, 1), 2);
		EXPECT_EQ(fovere::get(arr, 2), 3);
		EXPECT_EQ(fovere::get(arr, 3), 4);
		EXPECT_EQ(fovere::get(arr, 4), 5);

	}
}

TEST(Manipulators, tsInsert)
{
	{
		TestArray<int, 5> arr;

		fovere::insert(arr, 0, 5);
		fovere::insert(arr, 1, 4);
		fovere::insert(arr, 2, 3);
		fovere::insert(arr, 3, 2);
		fovere::insert(arr, 4, 1);

		EXPECT_EQ(fovere::get(arr, 0), 5);
		EXPECT_EQ(fovere::get(arr, 1), 4);
		EXPECT_EQ(fovere::get(arr, 2), 3);
		EXPECT_EQ(fovere::get(arr, 3), 2);
		EXPECT_EQ(fovere::get(arr, 4), 1);

	}
}

TEST(Manipulators, tsContain)
{
	{
		TestArray<int, 5> arr;

		fovere::insert(arr, 0, 1);
		fovere::insert(arr, 1, 2);
		fovere::insert(arr, 2, 3);
		fovere::insert(arr, 3, 2);
		fovere::insert(arr, 4, 4);

		EXPECT_TRUE(fovere::contain(arr, 1));
		EXPECT_TRUE(fovere::contain(arr, 2));
		EXPECT_TRUE(fovere::contain(arr, 3));
		EXPECT_TRUE(fovere::contain(arr, 4));
		EXPECT_FALSE(fovere::contain(arr, 5));
		EXPECT_FALSE(fovere::contain(arr, -1));

	}
}

TEST(Manipulators, tsCount)
{
	{
		TestArray<int, 5> arr;

		fovere::insert(arr, 0, 1);
		fovere::insert(arr, 1, 2);
		fovere::insert(arr, 2, 3);
		fovere::insert(arr, 3, 2);
		fovere::insert(arr, 4, 4);

		EXPECT_EQ(fovere::count(arr, 1), 1);
		EXPECT_EQ(fovere::count(arr, 2), 2);
		EXPECT_EQ(fovere::count(arr, 3), 1);
		EXPECT_EQ(fovere::count(arr, 4), 1);
		EXPECT_EQ(fovere::count(arr, 5), 0);
		EXPECT_EQ(fovere::count(arr, -1), 0);

	}
}

TEST(Manipulators, tsFind)
{
	{
		TestArray<int, 5> arr;

		fovere::insert(arr, 0, 1);
		fovere::insert(arr, 1, 2);
		fovere::insert(arr, 2, 3);
		fovere::insert(arr, 3, 2);
		fovere::insert(arr, 4, 4);

		EXPECT_EQ(fovere::find(arr,0, 5, 1).value(), 0);
		EXPECT_FALSE(fovere::find(arr, 1, 5, 1).has_value());

		EXPECT_EQ(fovere::find(arr,0, 5, 2).value(), 1);
		EXPECT_EQ(fovere::find(arr, 2, 5, 2).value(), 3);

		EXPECT_EQ(fovere::find(arr,0, 5, 3).value(), 2);
		EXPECT_EQ(fovere::find(arr, 0, 5, 4).value(), 4);
		EXPECT_FALSE(fovere::find(arr, 0, 2, 4).has_value());

		EXPECT_FALSE(fovere::find(arr, 0, 5, 5).has_value());
		EXPECT_FALSE(fovere::find(arr, 0, 5, -1).has_value());

		EXPECT_EQ(fovere::find(arr, 1).value(), 0);
		EXPECT_EQ(fovere::find(arr, 2).value(), 1);
		EXPECT_EQ(fovere::find(arr, 3).value(), 2);
		EXPECT_EQ(fovere::find(arr, 4).value(), 4);
		EXPECT_FALSE(fovere::find(arr, 5).has_value());
		EXPECT_FALSE(fovere::find(arr, -1).has_value());

	}
}

TEST(Manipulators, tsFill)
{

	{
		TestArray<int, 5> arr;

		fovere::insert(arr, 0, 1);
		fovere::insert(arr, 1, 2);
		fovere::insert(arr, 2, 3);
		fovere::insert(arr, 3, 2);
		fovere::insert(arr, 4, 4);

		fovere::fill(arr, -1);
		EXPECT_EQ(fovere::get(arr, 0), -1);
		EXPECT_EQ(fovere::get(arr, 1), -1);
		EXPECT_EQ(fovere::get(arr, 2), -1);
		EXPECT_EQ(fovere::get(arr, 3), -1);
		EXPECT_EQ(fovere::get(arr, 4), -1);

		fovere::fill(arr, 0, 3, 2);
		EXPECT_EQ(fovere::get(arr, 0), 2);
		EXPECT_EQ(fovere::get(arr, 1), 2);
		EXPECT_EQ(fovere::get(arr, 2), 2);
		EXPECT_EQ(fovere::get(arr, 3), -1);
		EXPECT_EQ(fovere::get(arr, 4), -1);

		fovere::fill(arr, 3, 5, 4);
		EXPECT_EQ(fovere::get(arr, 0), 2);
		EXPECT_EQ(fovere::get(arr, 1), 2);
		EXPECT_EQ(fovere::get(arr, 2), 2);
		EXPECT_EQ(fovere::get(arr, 3), 4);
		EXPECT_EQ(fovere::get(arr, 4), 4);

	}

}

TEST(Manipulators, tsAccumulate)
{

	{

		TestArray<int, 5> arr;

		fovere::insert(arr, 0, 1);
		fovere::insert(arr, 1, 2);
		fovere::insert(arr, 2, 3);
		fovere::insert(arr, 3, 2);
		fovere::insert(arr, 4, 4);

		EXPECT_EQ(fovere::accumulate(arr), 12);
		EXPECT_EQ(fovere::accumulate(arr, 0, 3), 6);
		EXPECT_EQ(fovere::accumulate(arr, 3, 5), 6);
		
		EXPECT_EQ(fovere::accumulate(arr, [](auto&& value)
			{
				return value * 2;
			}), 24);
		
		EXPECT_EQ(fovere::accumulate(arr, 0, 3, [](auto&& value)
			{
				return value * 8;
			}), 48);
		
		EXPECT_EQ(fovere::accumulate(arr, [](auto&& value)
			{
				return -value;
			}), -12);

	}

}