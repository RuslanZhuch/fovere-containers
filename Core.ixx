module;

#include <utility>

export module fovere.Core;
import fovere.Iterators.Core;

export namespace fovere::Core
{

	template <typename T>
	concept CtContainerContinuous = requires(T t, const T ct)
	{
		requires Iterators::CtSupportRangeBasedIterator<T>;
		{T::TVal};
		{ct.getLen()} -> std::convertible_to<size_t>;
		{ct[std::declval<size_t>()]} -> std::convertible_to<typename T::TVal>;
		{t[std::declval<size_t>()]} -> std::convertible_to<typename T::TVal&>;
	};

}
