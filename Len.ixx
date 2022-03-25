export module fovere.Len;

import fovere.Core;

export namespace fovere
{

	[[nodiscard]] constexpr auto len(const Core::CtContainerContinuous auto& container) noexcept
	{
		return container.getLen();
	}

};