module;

#include <utility>
#include <memory>
#include <algorithm>

export module fovere.Fill;
import fovere.Core;

export namespace fovere
{

	constexpr void fill(Core::CtContainerContinuous auto& container, size_t beginId, size_t endId, auto&& value) noexcept
	{
		for (size_t id{ beginId }; id < endId; ++id)
		{
			container[id] = value;
		}
	}

	constexpr void fill(Core::CtContainerContinuous auto& container, auto&& value) noexcept
	{
		fill(container, 0, container.getLen(), std::forward<decltype(value)>(value));
	}

};