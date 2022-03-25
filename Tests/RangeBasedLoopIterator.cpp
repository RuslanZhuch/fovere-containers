#include "pch.h"

import fovere.Iterators.Monodirectional;
import fovere.Iterators.Core;

static_assert(fovere::Iterators::CtIterator<fovere::Iterators::Monodir<int>>);
static_assert(fovere::Iterators::CtIterator<fovere::Iterators::Monodir<const int>>);

TEST(RangeBasedLoopIterator, tsBasicIncremantationAndCompare)
{

	int arr[5] = { 1, 2, 3, 4, 5 };

	fovere::Iterators::Monodir<int> iterBegin(&arr[0]);
	fovere::Iterators::Monodir<int> iterEnd(&arr[5]);

	EXPECT_NE(iterBegin, iterEnd);

	EXPECT_EQ(*iterBegin, 1);
	++iterBegin;
	EXPECT_EQ(*iterBegin, 2);
	EXPECT_NE(iterBegin, iterEnd);
	++iterBegin;
	EXPECT_EQ(*iterBegin, 3);
	EXPECT_NE(iterBegin, iterEnd);
	++iterBegin;
	EXPECT_EQ(*iterBegin, 4);
	EXPECT_NE(iterBegin, iterEnd);
	++iterBegin;
	EXPECT_EQ(*iterBegin, 5);
	EXPECT_NE(iterBegin, iterEnd);
	++iterBegin;
	EXPECT_EQ(iterBegin, iterEnd);

}

TEST(RangeBasedLoopIterator, tsConstIncremantationAndCompare)
{

	const int arr[5] = { 1, 2, 3, 4, 5 };

	fovere::Iterators::Monodir<const int> iterBegin(&arr[0]);
	fovere::Iterators::Monodir<const int> iterEnd(&arr[5]);

	EXPECT_NE(iterBegin, iterEnd);

	EXPECT_EQ(*iterBegin, 1);
	++iterBegin;
	EXPECT_EQ(*iterBegin, 2);
	EXPECT_NE(iterBegin, iterEnd);
	++iterBegin;
	EXPECT_EQ(*iterBegin, 3);
	EXPECT_NE(iterBegin, iterEnd);
	++iterBegin;
	EXPECT_EQ(*iterBegin, 4);
	EXPECT_NE(iterBegin, iterEnd);
	++iterBegin;
	EXPECT_EQ(*iterBegin, 5);
	EXPECT_NE(iterBegin, iterEnd);
	++iterBegin;
	EXPECT_EQ(iterBegin, iterEnd);

}

template <typename T>
class TestContainer
{
public:
	using TIter = typename fovere::Iterators::Monodir<T>;
public:
	auto begin() noexcept
	{
		return fovere::Iterators::Monodir(&arr[0]);
	}
	auto end() noexcept
	{
		return fovere::Iterators::Monodir(&arr[sizeof(arr) / sizeof(T)]);
	}

private:
	T arr[5] = { 1, 2, 3, 4, 5 };

};

static_assert(fovere::Iterators::CtSupportRangeBasedIterator<TestContainer<int>>);
static_assert(fovere::Iterators::CtSupportRangeBasedIterator<TestContainer<const int>>);

TEST(RangeBasedLoopIterator, tsUsing)
{

	TestContainer<int> cont;
	int val{ 10 };
	for (auto& el : cont)
	{
		el = val;
		val += 10;
	}
	EXPECT_EQ(val, 60);
	int expectVal{ 10 };
	for (const auto el : cont)
	{
		EXPECT_EQ(el, expectVal);
		expectVal += 10;
	}
	EXPECT_EQ(expectVal, 60);

}

TEST(RangeBasedLoopIterator, tsUsingConst)
{

	TestContainer<const int> cont;
	int expectVal{ 1 };
	for (const auto el : cont)
	{
		EXPECT_EQ(el, expectVal++);
	}
	EXPECT_EQ(expectVal, 6);

}