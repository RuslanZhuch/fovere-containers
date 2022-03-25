module;

#include <concepts>

export module fovere.Iterators.Core;

template <typename T>
concept CtConstConstructor = requires(T t, T t2, const typename T::TVal * v)
{
	{T(v)};
};
template <typename T>
concept CtNormalConstructor = requires(T t, T t2, typename T::TVal * v)
{
	{T(v)};
};

export namespace fovere::Iterators
{

	template <typename T>
	concept CtIterator = requires(T t, T t2)
	{
		requires CtConstConstructor<T> || CtNormalConstructor<T>;
		{T::TVal};
		{++t} -> std::convertible_to<T&>;
		{*t} -> std::convertible_to<typename T::TVal&>;
		{t == t2} -> std::convertible_to<bool>;
	};

	template <typename T>
	concept CtSupportRangeBasedIterator = requires(T t, const T ct)
	{
		requires CtIterator<typename T::TIter>;
		{t.begin()} -> std::convertible_to<typename T::TIter>;
		{t.end()} -> std::convertible_to<typename T::TIter>;
	};

};