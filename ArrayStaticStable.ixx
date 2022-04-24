module;

#include <cassert>
#include <utility>

export module fovere.Array.StaticStable;
export import fovere.Utils;

import :Iterator;

import hfog.Sources.Common;
import hfog.Algorithms.Pool;

export namespace fovere::Array
{

	template<hfog::Sources::CtSource Source, typename T, size_t capacity>
	class StaticStable
	{

		using Alloc = hfog::Algorithms::Pool<Source, sizeof(T), capacity,
			hfog::MemoryUtils::Align<sizeof(T)>>;

		struct ChunckNode
		{
			ChunckNode* next{ nullptr };
			ChunckNode* prev{ nullptr };
			T* data{ nullptr };
		};

	public:

		size_t append(auto&& value) noexcept 
			requires std::same_as<std::remove_cvref_t<decltype(value)>, std::remove_cv_t<T>>
		{

			if (this->localLen == capacity)
				return fovere::invalidIndex;

			const auto memBlock{ this->alloc.allocate(alignof(T)) };
			assert(memBlock.ptr != nullptr);
			
			++this->localLen;

			if (this->memoryEntry == nullptr)
			{
				this->memoryEntry = reinterpret_cast<std::remove_const_t<T>*>(memBlock.ptr);
			}

			const auto ptr{ reinterpret_cast<std::remove_cv_t<T>*>(memBlock.ptr) };
			*ptr = value;

			const auto offset{ ptr - this->memoryEntry };

			auto currChunck{ &this->chuncks[offset] };
			currChunck->data = ptr;
			currChunck->next = nullptr;
			currChunck->prev = this->lastChunck;
			if (this->lastChunck != nullptr)
				this->lastChunck->next = currChunck;
			if (this->firstChunck == nullptr)
				this->firstChunck = currChunck;
			this->lastChunck = currChunck;

			return offset;

		}

		void clear() noexcept
		{
			this->localLen = 0;
			this->firstChunck = nullptr;
			this->lastChunck = nullptr;
			this->alloc.deallocate();
		}

		void remove(size_t elId) noexcept
		{

			assert(elId < capacity);

			hfog::MemoryBlock memoryToDelete;
			memoryToDelete.ptr = reinterpret_cast<byte_t*>(this->memoryEntry + elId);
			memoryToDelete.size = sizeof(T);
			this->alloc.deallocate(memoryToDelete);

			auto currChunck{ &this->chuncks[elId] };
			auto prevChunck{ currChunck->prev };
			auto nextChunck{ currChunck->next };

			if (prevChunck != nullptr)
				prevChunck->next = nextChunck;
			if (nextChunck != nullptr)
				nextChunck->prev = prevChunck;

			currChunck->data = nullptr;
			currChunck->next = nullptr;
			currChunck->prev = nullptr;

			if (this->lastChunck == currChunck)
				this->lastChunck = prevChunck;
			if (this->firstChunck == currChunck)
				this->firstChunck = nextChunck;

			--this->localLen;

		}

		void precache() noexcept
		{
			T* sortedNodes[capacity];
			for (auto& n : sortedNodes)
				n = nullptr;

			auto currChunck{ this->firstChunck };
			while (currChunck != nullptr)
			{
				auto id{ currChunck->data - this->memoryEntry };
				sortedNodes[id] = currChunck->data;
				currChunck = currChunck->next;
			}

			size_t contId{ 0 };
			auto currData{ sortedNodes[contId] };
			auto currReal{ this->firstChunck };

			while (currReal != nullptr)
			{
				while((currData == nullptr) && (contId < capacity))
					currData = sortedNodes[++contId];
				if (currData != currReal->data)
				{
					currReal->data = currData;
				}
				currData = sortedNodes[++contId];
				currReal = currReal->next;
			}

		}

		[[nodiscard]] constexpr T operator[](size_t id) const noexcept
		{
			return *(this->memoryEntry + id);
		}

		[[nodiscard]] constexpr T& operator[](size_t id) noexcept
		{
			return *(this->memoryEntry + id);
		}

		[[nodiscard]] constexpr auto getLen() const noexcept
		{
			return this->localLen;
		}

		[[nodiscard]] constexpr auto begin() noexcept
		{
			return Iterator(this->firstChunck);
		}

		[[nodiscard]] constexpr auto end() noexcept
		{
			return Iterator<ChunckNode>(nullptr);
		}

	private:
		Alloc alloc;
		size_t localLen{ 0 };
		std::remove_const_t<T>* memoryEntry{ nullptr };

		ChunckNode chuncks[capacity];
		ChunckNode* firstChunck{ nullptr };
		ChunckNode* lastChunck{ nullptr };

	};

};