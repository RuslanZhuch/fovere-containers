module;
#include <cassert>
#include <memory>

export module fovere.Array.Universal;
export import fovere.Utils;

import fovere.Iterators.Monodirectional;

import hfog.Core;

export namespace fovere::Array
{

	template<hfog::CtAllocator Alloc, typename T>
	class Universal
	{
	public:
		using TVal = typename T;
		using TAlloc = typename Alloc;
	public:
		Universal(Alloc* alloc)
			:allocator(alloc)
		{
			this->memory = this->allocator->allocate(sizeof(T));
			assert(this->memory.ptr != nullptr);
			this->memoryEntry = reinterpret_cast<T*>(this->memory.ptr);
			this->capacity = 1;
		}

		[[nodiscard]] size_t append(const T& value) noexcept
		{

			if (!this->tryResizeUp())
				return invalidIndex;

			*(this->memoryEntry + this->localLen) = value;

			return this->localLen++;

		}

		[[nodiscard]] auto& operator[](size_t id) noexcept
		{
			assert(id < this->localLen);
			return *(this->memoryEntry + id);
		}

		[[nodiscard]] constexpr auto operator[](size_t id) const noexcept
		{
			assert(id < this->localLen);
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

			this->tryResizeDown();

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
			
			this->tryResizeDown();

		}

		[[nodiscard]] bool insert(size_t insertAfterId, T&& value)
		{

			assert(this->localLen != 0);
			assert(insertAfterId < this->localLen);

			if (!this->tryResizeUp())
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
		}

		[[nodiscard]] constexpr auto begin() const noexcept
		{
			return Iterators::Monodir<T>(this->memoryEntry);
		}

		[[nodiscard]] constexpr auto end() const noexcept
		{
			return Iterators::Monodir<T>(this->memoryEntry + this->localLen);
		}

	private:
		[[nodiscard]] bool tryResizeUp() noexcept
		{

			if (this->localLen < this->capacity)
				return true;

			const auto newCapacity{ this->capacity * 2 };
			const auto memBlock = [&]()
			{
				const auto memBlockFull{ allocator->allocate(newCapacity * sizeof(T)) };
				if (memBlockFull.ptr != nullptr)
					return memBlockFull;
				return allocator->allocate((this->capacity + 1) * sizeof(T));
			}();

			if (memBlock.ptr == nullptr)
				return false;

			this->capacity = (memBlock.size / sizeof(T));

			std::memcpy(memBlock.ptr, this->memory.ptr, this->memory.size);
			this->allocator->deallocate(this->memory);
			this->memory = memBlock;
			this->memoryEntry = reinterpret_cast<T*>(this->memory.ptr);
			
			return true;

		}

		void tryResizeDown() noexcept
		{

			if (this->localLen >= this->capacity / 4)
				return;

			this->capacity /= 4;
			const auto memBlock{ this->allocator->allocate(this->capacity * sizeof(T)) };
			if (memBlock.ptr == nullptr)
				return;
			std::memcpy(memBlock.ptr, this->memory.ptr, memBlock.size);
			this->allocator->deallocate(this->memory);
			this->memory = memBlock;
			this->memoryEntry = reinterpret_cast<T*>(this->memory.ptr);

		}

	private:
		Alloc* allocator{ nullptr };
		hfog::MemoryBlock memory;
		T* memoryEntry{ nullptr };
		size_t localLen{ 0 };
		size_t capacity{ 0 };

	};

};