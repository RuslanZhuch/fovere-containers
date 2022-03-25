export module fovere.Get;
import fovere.Core;

export namespace fovere
{

	[[nodiscard]] constexpr auto get(const Core::CtContainerContinuous auto& container, size_t id) noexcept
	{
		return container[id];
	}

};