export module fovere.Len;

export namespace fovere
{

	[[nodiscard]] constexpr auto len(const auto& container) noexcept
	{
		return container.getLen();
	}

};