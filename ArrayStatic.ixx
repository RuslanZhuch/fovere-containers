module;

#include <cstddef>
#include <utility>
#include <cassert>

export module fovere.Array.Static;
export import fovere.Utils;

import fovere.Iterators.Monodirectional;

import hfogAlloc;

export namespace fovere::Array
{

	template <typename T, size_t maxElements>
	class Static
	{
	public:
		using TIter = fovere::Iterators::Monodir<T>;
		using TVal = typename T;

	public:
		constexpr Static() noexcept = default;
		constexpr Static(size_t initLen) noexcept
			:localLen(initLen)
		{

			assert(localLen <= maxElements);

			auto memBlock{ this->appendSize(initLen) };
			this->beginPtr = reinterpret_cast<T*>(memBlock.ptr);

		}

		constexpr Static(size_t initLen, const T& initValue) noexcept
			:Static::Static(initLen)
		{

			size_t id{ 0 };
			while (id != initLen)
			{
				*(this->beginPtr + id++) = initValue;
			}

		}

		[[nodiscard]] constexpr auto operator()() noexcept
		{
			return this->beginPtr;
		}

		[[nodiscard]] constexpr auto getLen() const noexcept
		{
			return this->localLen;
		}

		[[nodiscard]] auto& operator[](size_t id) noexcept
		{
			size_t offset{ id };
			if (id >= this->localLen)
			{
				const auto diff{ id - this->localLen + 1 };
				auto memBlock{ this->appendSize(diff) };
				this->localLen = id + 1;

				if (this->beginPtr == nullptr)
				{
					this->beginPtr = reinterpret_cast<T*>(memBlock.ptr);
				}

				if (memBlock.ptr == nullptr || memBlock.size == 0)
				{
					assert(false);
					offset = 0;
				}
			}
			return *(this->beginPtr + offset);
		}

		[[nodiscard]] constexpr auto operator[](size_t id) const noexcept
		{
			return *(this->beginPtr + id);
		}

		[[nodiscard]] constexpr auto insert(size_t afterId, T&& value) noexcept
		{
			const auto insertId{ afterId + 1 };
			const auto freeGap{ maxElements - this->localLen };
			const auto bCanResize{ (freeGap > 0) && (afterId < this->localLen) };
			if (!bCanResize)
				return invalidIndex;

			auto backId{ this->localLen };
			while (backId > afterId)
			{
				this->operator[](backId) = *(this->beginPtr + backId - 1);
				backId--;
			}
			*(this->beginPtr + insertId) = value;
			return insertId;
		}

		constexpr void remove(size_t removeId) noexcept
		{
			assert(removeId < this->localLen);
			const auto lastId{ this->localLen - 1 };
			auto currId{ lastId };
			while (currId > removeId)
			{
				*(this->beginPtr + currId - 1) = *(this->beginPtr + currId);
				--currId;
			}
			hfog::MemoryBlock memBlock;
			memBlock.ptr = reinterpret_cast<byte_t*>(this->beginPtr + lastId);
			memBlock.size = alignof(T);
			this->alloc.deallocate(memBlock);
			--this->localLen;
		}

		[[nodiscard]] constexpr auto begin() noexcept
		{
			return TIter(this->beginPtr);
		}

		[[nodiscard]] constexpr auto end() noexcept
		{
			return TIter(this->beginPtr + this->localLen);
		}

	private:
		[[nodiscard]] constexpr auto appendSize(size_t elementsToAdd)
		{
			const auto needAllocBytes{ elementsToAdd * alignof(T) };
			return this->alloc.allocate(needAllocBytes);
		}

	private:
		hfog::Alloc::Stack<alignof(T), alignof(T) * maxElements> alloc;
		T* beginPtr{ nullptr };
		size_t localLen{ 0 };
	};

};