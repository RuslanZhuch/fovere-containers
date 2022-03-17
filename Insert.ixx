module;

#include <utility>

export module fovere.Insert;

export namespace fovere
{

	constexpr void insert(auto& container, size_t id, auto&& value) noexcept
	{
		container[id] = std::forward<decltype(value)>(value);
	}

};