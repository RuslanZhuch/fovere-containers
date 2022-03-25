export module fovere.Contain;
import fovere.Core;

export namespace fovere
{

	[[nodiscard]] constexpr auto contain(const Core::CtContainerContinuous auto& container, auto&& value) noexcept
	{
		for (size_t id{ 0 }; id < container.getLen(); ++id)
		{
			if (container[id] == value)
				return true;
		}
		return false;
	}

};