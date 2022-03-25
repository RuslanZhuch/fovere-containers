export module fovere.Array.StaticStable:Iterator;

namespace fovere::Array
{
	template <typename T>
	class IteratorConst
	{

	public:
		using TVal = typename T;
	public:
		IteratorConst(T* p) noexcept
			:entry(p)
		{}
		auto& operator++() const noexcept
		{
			entry = entry->next;
			return *this;
		}
		auto operator->() const noexcept
		{
			return entry->data;
		}
		auto& operator*() const noexcept
		{
			return *this->operator->();
		}
		friend auto operator==(const IteratorConst& left, const IteratorConst& right) noexcept
		{
			return left.entry == right.entry;
		}
	private:
		mutable T* entry;
		
	};
};

export namespace fovere::Array
{

	template <typename T>
	class Iterator : public IteratorConst<T>
	{
		using TBase = IteratorConst<T>;
	public:
//		Iterator(T* p) noexcept
//			:TBase(p)
//		{}
//		auto& operator++() noexcept
//		{
//			TBase::operator++();
//			return *this;
//		}
//		auto& operator*() noexcept
//		{
//			return const_cast<T&>(TBase::operator*());
//		}

	};

};