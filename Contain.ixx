export module fovere.Contain;

export namespace fovere
{

	[[nodiscard]] constexpr auto contain(const auto& container, auto&& value) noexcept
	{
		for (size_t id{ 0 }; id < container.getLen(); ++id)
		{
			if (container[id] == value)
				return true;
		}
		return false;
	}

};