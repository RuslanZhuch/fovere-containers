module;

#include <utility>

export module fovere.Fill;

export namespace fovere
{

	constexpr void fill(auto& container, size_t beginId, size_t endId, auto&& value) noexcept
	{
		for (size_t id{ beginId }; id < endId; ++id)
		{
			container[id] = value;
		}
	}

	constexpr void fill(auto& container, auto&& value) noexcept
	{
		fill(container, 0, container.getLen(), std::forward<decltype(value)>(value));
	}

};