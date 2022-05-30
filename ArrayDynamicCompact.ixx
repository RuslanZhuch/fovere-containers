module;
#include <cassert>

export module fovere.Array.DynamicCompact;
export import fovere.Utils;

import hfog.Core;

import fovere.Iterators.Monodirectional;

export namespace fovere::Array
{

	template<hfog::CtAllocator Alloc, typename T>
	class DynamicCompact
	{
		using TIter = fovere::Iterators::Monodir<T>;
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
			if (id != this->localLen - 1)
			{
				auto currId{ id };
				while (currId < this->localLen - 1)
				{
					*(this->memoryEntry + currId) = *(this->memoryEntry + currId + 1);
					++currId;
				}
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

			auto currSrcId{ endId };
			auto currTargetId{ beginId };
			while (currSrcId < this->localLen)
			{
				*(this->memoryEntry + currTargetId) = *(this->memoryEntry + currSrcId);
				++currTargetId;
				++currSrcId;
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

			auto currId{ this->localLen++ };
			while (currId > insertAfterId)
			{
				*(this->memoryEntry + currId) = *(this->memoryEntry + currId - 1);
				--currId;
			}

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