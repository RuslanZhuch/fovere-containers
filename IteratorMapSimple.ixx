module;
#include <utility>

export module fovere.Map.Simple:Iterator;
import fovere.Iterators.Monodirectional;

namespace fovere::Map
{
	template <typename IterKeys, typename IterItems>
	class IteratorConst
	{

	protected:
		struct Data
		{
			IterKeys::TVal* key;
			IterItems::TVal* item;
		};

	public:
		IteratorConst(IterKeys keys, IterItems items) noexcept
			:
			keysIterator(keys),
			itemsIterator(items)
		{}
		auto& operator++() const noexcept
		{
			++this->keysIterator;
			++this->itemsIterator;
			return *this;
		}
		auto& operator*() const noexcept
		{
			this->outData.key = &*this->keysIterator;
			this->outData.item = &*this->itemsIterator;

			return this->outData;
		}
		friend auto operator==(const IteratorConst& left, const IteratorConst& right) noexcept
		{
			return left.keysIterator == right.keysIterator && 
				left.itemsIterator == right.itemsIterator;
		}
	private:
		mutable IterKeys keysIterator;
		mutable IterItems itemsIterator;

		mutable Data outData;

	};
};

export namespace fovere::Map
{

	template <typename IterKeys, typename IterItems>
	class Iterator : public IteratorConst<IterKeys, IterItems>
	{
		using TBase = IteratorConst<IterKeys, IterItems>;
	public:
		template <typename IterKeys, typename IterItems>
		Iterator(IterKeys keys, IterItems items) noexcept
			:TBase(keys, items)
		{}
		auto& operator++() noexcept
		{
			TBase::operator++();
			return *this;
		}
		auto& operator*() noexcept
		{
			return const_cast<TBase::Data&>(TBase::operator*());
		}

	};

};