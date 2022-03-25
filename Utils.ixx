module;

#include <numeric>

export module fovere.Utils;

export namespace fovere
{
	inline constexpr auto invalidIndex{ std::numeric_limits<size_t>::max() };
}; 