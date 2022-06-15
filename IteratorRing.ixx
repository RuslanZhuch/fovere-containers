module;
#include <utility>
#include <span>

export module fovere.RingBuffer:Iterator;

namespace fovere::Buffer
{
	template <typename T, typename MemBuffer>
	class IteratorConst
	{

	public:
		IteratorConst(std::span<MemBuffer> buffer, size_t beginId, size_t endId, size_t startId) noexcept
			:
			memBuffer(buffer),
			beginId(beginId),
			endId(endId),
			currId(startId)
		{}
		auto& operator++() const noexcept
		{
			this->currId = currId + 1;
			if (this->currId == this->memBuffer.size())
				this->currId = 0;

			return *this;
		}
		auto& operator*() const noexcept
		{
			return *reinterpret_cast<T*>((memBuffer.data() + this->currId)->ptr);
		}
		friend auto operator==(const IteratorConst& left, const IteratorConst& right) noexcept
		{
			return left.currId == right.currId;
		}
	private:
		std::span<MemBuffer> memBuffer;
		size_t beginId{};
		size_t endId{};

		mutable size_t currId{};

	};
};

export namespace fovere::Buffer
{

	template <typename T, typename MemBuffer>
	class Iterator : public IteratorConst<T, MemBuffer>
	{
		using TBase = IteratorConst<T, MemBuffer>;
	public:
		Iterator(std::span<MemBuffer> buffer, size_t beginId, size_t endId, size_t startId) noexcept
			:TBase(buffer, beginId, endId, startId)
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