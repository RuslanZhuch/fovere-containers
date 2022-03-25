module;

#include <compare>

export module fovere.Iterators.Monodirectional;

template <typename T>
class MonodirConst
{
public:
	using TVal = typename T;
public:
	MonodirConst(const T* p) noexcept
		:ptr(p)
	{}
	auto& operator++() const noexcept
	{
		++offset;
		return *this;
	}
	auto operator->() const noexcept
	{
		return ptr + offset;
	}
	auto& operator*() const noexcept
	{
		return *this->operator->();
	}
	friend auto operator==(const MonodirConst& left, const MonodirConst& right) noexcept
	{
		return left.ptr + left.offset == right.ptr + right.offset;
	}
private:
	const T* ptr;
	mutable size_t offset{ 0 };
};

export namespace fovere::Iterators
{

	template <typename T>
	class Monodir : public MonodirConst<T>
	{
		using TBase = MonodirConst<T>;
	public:
		Monodir(T* p) noexcept
			:TBase(p)
		{}
		auto& operator++() noexcept
		{
			TBase::operator++();
			return *this;
		}
		auto& operator*() noexcept
		{
			return const_cast<T&>(TBase::operator*());
		}
	};

};