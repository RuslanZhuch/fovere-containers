module;

#include <utility>

export module fovere.Insert;
import fovere.Core;

export namespace fovere
{

	constexpr void insert(Core::CtContainerContinuous auto& container, size_t id, auto&& value) noexcept
	{
		container[id] = std::forward<decltype(value)>(value);
	}

};