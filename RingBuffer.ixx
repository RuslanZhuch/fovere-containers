module;
#include <cassert>
#include <memory>
#include <span>

export module fovere.RingBuffer;
export import fovere.Utils;

import hfog.Core;
import hfog.Algorithms.Ring;
import hfog.Sources.Ext;

import :Iterator;

export namespace fovere::Buffer
{

	template<typename T, size_t MAX_ELEMENTS, hfog::CtAllocator Alloc>
	class Ring
	{

//		template <hfog::Sources::CtSource Source>
		using InnerAlloc = hfog::Algorithms::Ring<hfog::Sources::External<>, sizeof(T), hfog::MemoryUtils::Align<sizeof(T)>>;
		using TIter = Buffer::Iterator<T, hfog::MemoryBlock>;

	public:
		using TVal = typename T;
		using TAlloc = typename Alloc;
	public:
		Ring() = delete;

		Ring(Alloc* alloc, mem_t capacity)
			:allocator(alloc->allocate(capacity))
		{
			std::memset(this->memBlocks, 0, sizeof(this->memBlocks));
		}

		[[nodiscard]] T* append(size_t size = 1) noexcept
		{

			if (this->localLen == MAX_ELEMENTS)
				return nullptr;

			const auto appendSize{ sizeof(T) * size };
			const auto memBlock{ allocator.allocate(appendSize) };
			if (memBlock.ptr == nullptr)
				return nullptr;

//			const auto copySize{ std::min(memBlock.size, appendSize) };
//			std::memcpy(memBlock.ptr, &value, copySize);

			this->memBlocks[this->addPoint] = memBlock;
			this->addPoint = this->shiftPoint(this->addPoint);

			++this->localLen;

			return reinterpret_cast<T*>(memBlock.ptr);

		}

		[[nodiscard]] constexpr auto getLen() const noexcept
		{
			return this->localLen;
		}

		[[nodiscard]] mem_t copyAndPop(T* dest, size_t sizeToCopy) noexcept
		{

			if (this->localLen == 0)
				return 0;

			const auto memBlock{ this->memBlocks[this->removePoint]};
			this->removePoint = this->shiftPoint(this->removePoint);

			const auto copyBytes{ std::min(sizeToCopy, memBlock.size) };
			std::memcpy(dest, memBlock.ptr, copyBytes);

			this->allocator.deallocate(memBlock);
			--this->localLen;

			return copyBytes;

		}

		void clear() noexcept
		{
			this->localLen = 0;
			this->addPoint = 0;
			this->removePoint = 0;
			this->allocator.deallocate();
		}

		[[nodiscard]] constexpr auto begin() noexcept
		{
			return TIter(std::span(this->memBlocks), this->addPoint, this->removePoint, this->removePoint);
		}

		[[nodiscard]] constexpr auto end() noexcept
		{
			return TIter(std::span(this->memBlocks), this->addPoint, this->removePoint, this->addPoint);
		}

	private:
		mem_t shiftPoint(mem_t point) const noexcept
		{
			const auto newPoint{ point + 1 };
			if (newPoint == MAX_ELEMENTS)
				return 0;
			return newPoint;
		}

	private:
		size_t localLen{ 0 };
//		Alloc* extAllocator{ nullptr };
		InnerAlloc allocator;

		hfog::MemoryBlock memBlocks[MAX_ELEMENTS];

		mem_t addPoint{ 0 };
		mem_t removePoint{ 0 };

	};

};