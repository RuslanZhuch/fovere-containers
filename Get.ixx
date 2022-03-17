export module fovere.Get;

export namespace fovere
{

	[[nodiscard]] constexpr auto get(const auto& container, size_t id) noexcept
	{
		return container[id];
	}

};