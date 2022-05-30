module;

#include <cstddef>
#include <utility>

export module fovere.Array.Fixed;
import fovere.Iterators.Monodirectional;

export namespace fovere::Array
{

	template <typename T, size_t numOfElements>
	class Fixed
	{
	public:
		using TIter = fovere::Iterators::Monodir<T>;
		using TVal = typename T;
	public:
		Fixed() noexcept = default;
		constexpr Fixed(T initVal) noexcept
		{
			for (auto& el : this->data)
			{
				el = initVal;
			}
		}

		[[nodiscard]] constexpr auto getLen() const noexcept
		{
			return numOfElements;
		}

		[[nodiscard]] constexpr auto& operator[](auto id) noexcept
		{
			return this->data[id];
		}

		[[nodiscard]] constexpr auto operator[](auto id) const noexcept
		{
			return this->data[id];
		}

		[[nodiscard]] constexpr auto begin() noexcept
		{
			return TIter(&this->data[0]);
		}

		[[nodiscard]] constexpr auto end() noexcept
		{
			return TIter(&this->data[numOfElements]);
		}

	private:
		T data[numOfElements];

	};

}