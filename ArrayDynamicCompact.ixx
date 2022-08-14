module;
#include <cassert>
#include <memory>
#include <utility>

export module fovere.Array.DynamicCompact;
export import fovere.Utils;

import hfog.Core;
import hfog.Algorithms.Stack;
import hfog.Sources.Ext;

import fovere.Iterators.Monodirectional;

export namespace fovere::Array
{

	template<hfog::CtAllocator Alloc, typename T>
	class DynamicCompact
	{
		using TIter = fovere::Iterators::Monodir<T>;
	public:
		using TVal = typename T;
		using TAlloc = typename Alloc;
	public:
		DynamicCompact() = delete;

		template <hfog::CtAllocator ExtAlloc>
		DynamicCompact(ExtAlloc* alloc, mem_t capacity)
			:allocator(alloc->allocate(capacity))
		{
			const auto memBlock{ allocator.allocate(0) };
			this->memoryEntry = reinterpret_cast<T*>(memBlock.ptr);
		}

		[[nodiscard]] size_t append(T value) noexcept
		{
			const auto memBlock{ allocator.allocate(sizeof(T)) };
			if (memBlock.ptr == nullptr)
				return invalidIndex;
			//if (this->memoryEntry == nullptr)
			//	this->memoryEntry = reinterpret_cast<T*>(memBlock.ptr);
			//std::memcpy(memBlock.ptr, &value, sizeof(T));
			//*(this->memoryEntry + this->localLen) = value;
			*(reinterpret_cast<T*>(memBlock.ptr)) = value;

			//Remove local len 
			//++this->localLen;
			const auto index{ (memBlock.ptr - reinterpret_cast<byte_t*>(this->memoryEntry)) / sizeof(T) };
			return index;
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
			const auto tailPtr{ allocator.allocate(0).ptr };
			const auto len{ (tailPtr - this->memoryEntry) / sizeof(T) };
			return len;
		}

		void remove(size_t id) noexcept
		{

			const auto tailPtr{ allocator.allocate(0).ptr };
			const auto len{ (tailPtr - this->memoryEntry) / sizeof(T) };

			assert(id < len);
			const auto copyBegin{ id + 1 };
			const auto elementsToCopy{ len - 1 - id };
			const auto elementsToCopyInBytes{ elementsToCopy * sizeof(T) };

			if (id != len - 1)
			{
				std::memcpy(this->memoryEntry + id, this->memoryEntry + copyBegin, elementsToCopyInBytes);
			}

			//--this->localLen;
			const auto newLen{ len - 1 };
			hfog::MemoryBlock data;
			data.ptr = reinterpret_cast<byte_t*>(this->memoryEntry + newLen);
			data.size = sizeof(T);
			this->allocator.deallocate(data);

		}

		void remove(size_t beginId, size_t endId) noexcept
		{

			const auto tailPtr{ allocator.allocate(0).ptr };
			const auto len{ (tailPtr - this->memoryEntry) / sizeof(T) };

			assert(endId <= len);
			assert(beginId < endId);

			const auto copyBegin{ endId };
			const auto elementsToCopy{ len - endId };
			const auto elementsToCopyInBytes{ elementsToCopy * sizeof(T) };

			if (copyBegin < len)
			{
				memcpy(this->memoryEntry + beginId, this->memoryEntry + copyBegin, elementsToCopyInBytes);
			}

			const auto elementsToDelete{ endId - beginId };
			const auto newLen{ len - elementsToDelete };
			hfog::MemoryBlock data;
			data.ptr = reinterpret_cast<byte_t*>(this->memoryEntry + newLen);
			data.size = elementsToDelete * sizeof(T);
			this->allocator.deallocate(data);

		}

		[[nodiscard]] bool insert(size_t insertAfterId, T&& value)
		{

			//assert(this->localLen != 0);
			//assert(insertAfterId < this->localLen);

			const auto memBlock{ this->allocator.allocate(sizeof(T)) };
			if (memBlock.ptr == nullptr)
				return false;

			//this->localLen++;
			const auto tailPtr{ allocator.allocate(0).ptr };
			const auto len{ (tailPtr - this->memoryEntry) / sizeof(T) };
			const auto elementsToCopy{ len - insertAfterId - 2 };
			const auto elementsToCopyInBytes{ elementsToCopy * sizeof(T) };
			std::memcpy(this->memoryEntry + insertAfterId + 2, this->memoryEntry + insertAfterId + 1, elementsToCopyInBytes);

			*(this->memoryEntry + insertAfterId + 1) = value;

			return true;

		}

		void clear() noexcept
		{
			//this->localLen = 0;
			this->allocator.deallocate();
		}

		[[nodiscard]] constexpr auto begin() noexcept
		{
			return TIter(memoryEntry);
		}

		[[nodiscard]] constexpr auto end() noexcept
		{
			const auto memBlock{ allocator.allocate(0) };
			return TIter(memBlock.ptr);
		}

	private:
		//size_t localLen{ 0 };
		T* memoryEntry{ nullptr };
		mutable Alloc allocator{ nullptr };
	};

};