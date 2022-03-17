export module fovere.Count;

export namespace fovere
{

	[[nodiscard]] constexpr size_t count(const auto& container, auto&& value)
	{

		size_t count{ 0 };

		for (size_t id{ 0 }; id < container.getLen(); ++id)
		{
			if (container[id] == value)
				++count;
		}

		return count;

	}

};