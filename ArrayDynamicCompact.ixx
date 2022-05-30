module;
#include <cassert>
#include <memory>

export module fovere.Array.DynamicCompact;
export import fovere.Utils;

import hfog.Core;
import hfog.Algorithms.Stack;
import hfog.Sources.Common;

import fovere.Iterators.Monodirectional;

export namespace fovere::Array
{

	template<typename T, hfog::Sources::CtSource Source>
	class DynamicCompact
	{
		using TIter = fovere::Iterators::Monodir<T>;
		using Alloc = hfog::Algorithms::Stack<Source, sizeof(T), hfog::MemoryUtils::Align<sizeof(T)>>;
	public:
		using TVal = typename T;
		using TAlloc = typename Alloc;
	public:
		DynamicCompact() = delete;

		DynamicCompact(Alloc* alloc)
			:allocator(alloc)
		{}

		[[nodiscard]] size_t append(T&& value) noexcept
		{
			const auto memBlock{ allocator->allocate(sizeof(T)) };
			if (memBlock.ptr == nullptr)
				return invalidIndex;
			if (this->memoryEntry == nullptr)
				this->memoryEntry = reinterpret_cast<T*>(memBlock.ptr);
			*(this->memoryEntry + this->localLen) = value;

			return this->localLen++;
		}

		[[nodiscard]] auto& operator[](size_t id) noexcept
		{
			return *(this->memoryEntry + id);
		}

		[[nodiscard]] constexpr auto operator[](size_t id) const noexcept
		{
			return *(this->memoryEntry + id);
		}

		[[nodiscard]] constexpr auto getLen() const noexcept
		{
			return this->localLen;
		}

		void remove(size_t id) noexcept
		{

			assert(id < this->localLen);
			const auto copyBegin{ id + 1 };
			const auto elementsToCopy{ this->localLen - 1 - id };
			const auto elementsToCopyInBytes{ elementsToCopy * sizeof(T) };

			if (id != this->localLen - 1)
			{
				std::memcpy(this->memoryEntry + id, this->memoryEntry + copyBegin, elementsToCopyInBytes);
			}

			--this->localLen;
			hfog::MemoryBlock data;
			data.ptr = reinterpret_cast<byte_t*>(this->memoryEntry + this->localLen);
			data.size = sizeof(T);
			this->allocator->deallocate(data);

		}

		void remove(size_t beginId, size_t endId) noexcept
		{

			assert(endId <= this->localLen);
			assert(beginId < endId);

			const auto copyBegin{ endId };
			const auto elementsToCopy{ this->localLen - endId };
			const auto elementsToCopyInBytes{ elementsToCopy * sizeof(T) };

			if (copyBegin < this->localLen)
			{
				memcpy(this->memoryEntry + beginId, this->memoryEntry + copyBegin, elementsToCopyInBytes);
			}

			const auto elementsToDelete{ endId - beginId };
			this->localLen -= elementsToDelete;
			hfog::MemoryBlock data;
			data.ptr = reinterpret_cast<byte_t*>(this->memoryEntry + this->localLen);
			data.size = elementsToDelete * sizeof(T);
			this->allocator->deallocate(data);

		}

		[[nodiscard]] bool insert(size_t insertAfterId, T&& value)
		{

			assert(this->localLen != 0);
			assert(insertAfterId < this->localLen);

			const auto memBlock{ this->allocator->allocate(sizeof(T)) };
			if (memBlock.ptr == nullptr)
				return false;

			this->localLen++;
			const auto elementsToCopy{ this->localLen - insertAfterId - 2 };
			const auto elementsToCopyInBytes{ elementsToCopy * sizeof(T) };
			std::memcpy(this->memoryEntry + insertAfterId + 2, this->memoryEntry + insertAfterId + 1, elementsToCopyInBytes);

			*(this->memoryEntry + insertAfterId + 1) = value;

			return true;

		}

		void clear() noexcept
		{
			this->localLen = 0;
			this->allocator->deallocate();
		}

		[[nodiscard]] constexpr auto begin() noexcept
		{
			return TIter(memoryEntry);
		}

		[[nodiscard]] constexpr auto end() noexcept
		{
			return TIter(memoryEntry + this->localLen);
		}

	private:
		size_t localLen{ 0 };
		T* memoryEntry{ nullptr };
		Alloc* allocator{ nullptr };
	};

};