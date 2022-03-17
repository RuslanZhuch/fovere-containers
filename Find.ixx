module;

#include <utility>
#include <optional>

export module fovere.Find;

export namespace fovere
{

	[[nodiscard]] constexpr std::optional<size_t> find(const auto& container, size_t beginId,
		size_t endId, auto&& value)
	{
		for (size_t id{ beginId }; id < endId; ++id)
		{
			if (container[id] == value)
				return id;
		}
		return std::nullopt;
	}

	[[nodiscard]] constexpr auto find(const auto& container, auto&& value)
	{
		return find(container, 0, container.getLen(), std::forward<decltype(value)>(value));
	}


};