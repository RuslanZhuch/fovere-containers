module;

#include <utility>

export module fovere.Accumulate;

export namespace fovere
{

	template <typename TContainer, typename TFunc>
	[[nodiscard]] constexpr auto accumulate(const TContainer& container, size_t beginId, size_t endId,
		TFunc func) noexcept
	{

		using TReturn = TContainer::TVal;
		TReturn result{ 0 };

		for (size_t id{ beginId }; id < endId; ++id)
		{
			result += func(container[id]);
		}

		return result;

	}

	template <typename TContainer>
	[[nodiscard]] constexpr auto accumulate(const TContainer& container, size_t beginId, size_t endId) noexcept
	{

		const auto func = [](auto&& value) {return value; };
		return accumulate<TContainer, decltype(func)>(container, beginId, endId, func);

	}

	template <typename TContainer>
	[[nodiscard]] constexpr auto accumulate(const TContainer& container, auto func) noexcept
	{
		return accumulate<TContainer, decltype(func)>(container, 0, container.getLen(), func);
	}

	template <typename TContainer>
	[[nodiscard]] constexpr auto accumulate(const TContainer& container) noexcept
	{
		return accumulate<TContainer>(container, 0, container.getLen());
	}

};