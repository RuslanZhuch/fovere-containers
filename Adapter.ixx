module;
#include <cassert>

export module fovere.Adapter;

import hfogCore;

export namespace fovere
{

	template<hfog::CtAllocator Alloc, typename T>
	class Adapter
	{
	public:
		[[nodiscard]] size_t append(T&& value) noexcept
		{
			const auto memBlock{ allocator.allocate(alignof(T)) };
			assert(memBlock.ptr != nullptr);
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
				auto currId{ this->localLen - 1 };
				while (currId > id)
				{
					*(this->memoryEntry + currId - 1) = *(this->memoryEntry + currId);
					--currId;
				}
			}

			--this->localLen;
			hfog::MemoryBlock data;
			data.ptr = reinterpret_cast<byte_t*>(this->memoryEntry + this->localLen);
			data.size = alignof(T);
			this->allocator.deallocate(data);

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
			data.size = elementsToDelete * alignof(T);
			this->allocator.deallocate(data);

		}

		void insert(size_t insertAfterId, T&& value)
		{

			assert(this->localLen != 0);
			assert(insertAfterId < this->localLen);

			const auto memBlock{ this->allocator.allocate(alignof(T)) };
			assert(memBlock.ptr != nullptr);

			auto currId{ this->localLen++ };
			while (currId > insertAfterId)
			{
				*(this->memoryEntry + currId) = *(this->memoryEntry + currId - 1);
				--currId;
			}

			*(this->memoryEntry + insertAfterId + 1) = value;

		}

		void clear() noexcept
		{
			this->localLen = 0;
			this->allocator.deallocate();
		}

	private:
		Alloc allocator;
		T* memoryEntry{ nullptr };
		size_t localLen{ 0 };
	};

};