module;
#include <memory>
#include <mutex>

export module fovere.Transport.Queue;
export import fovere.Utils;

import hfog.Core;
import hfog.Algorithms.Ring;
import hfog.Sources.Ext;

import hfog.Core;

export namespace fovere::Transport
{

	template<typename T, hfog::CtAllocator Alloc>
	class Queue
	{

		static constexpr auto ALIGNMENT{ sizeof(T) };
		using InnerAlloc = hfog::Algorithms::Ring<hfog::Sources::External<>, sizeof(T), hfog::MemoryUtils::Align<ALIGNMENT>>;
	private:
		struct Node
		{
			T data;
			Node* next{ nullptr };
		};

	public:
		using TVal = typename T;
		using TAlloc = typename Alloc;
	public:
		Queue() = delete;

		Queue(Alloc* alloc, size_t capacity)
			:allocator(alloc->allocate(hfog::MemoryUtils::Align<ALIGNMENT>::compute(capacity * sizeof(Node))))
		{	}

		[[nodiscard]] bool push(const T& data) noexcept
		{

			std::scoped_lock(this->mt);

			const auto memBlock{ this->allocator.allocate(sizeof(Node)) };
			if (memBlock.ptr == nullptr)
				return false;

			this->nodeSize = memBlock.size;

			auto node = reinterpret_cast<Node*>(memBlock.ptr);
			node->next = nullptr;

			std::memcpy(&node->data, &data, sizeof(data));
			
			if (this->head == nullptr)
			{
				this->head = node;
				this->tail = this->head;
			}
			else
			{
				auto prevTail{ this->tail };
				prevTail->next = node;
				this->tail = node;
			}

			++this->localLen;

			return true;

		}

		[[nodiscard]] constexpr auto getLen() const noexcept
		{
			return this->localLen;
		}

		[[nodiscard]] bool pop(T* dest) noexcept
		{

			std::scoped_lock(this->mt);

			if (this->head == nullptr)
				return false;

			auto node{ this->head };

			this->head = node->next;
			if (this->head == nullptr)
				this->tail = nullptr;

			std::memcpy(dest, &node->data, sizeof(T));

			hfog::MemoryBlock blockToDeallocate;
			blockToDeallocate.ptr = reinterpret_cast<byte_t*>(node);
			blockToDeallocate.size = hfog::MemoryUtils::Align<ALIGNMENT>::compute(sizeof(Node));

			this->allocator.deallocate(blockToDeallocate);
			--this->localLen;

			return true;

		}

//		void clear() noexcept
//		{
//			this->localLen = 0;
//			this->head = nullptr;
//			this->tail = nullptr;
//			this->allocator.deallocate();
//		}

	private:
		size_t localLen{ 0 };
		size_t nodeSize{ 0 };
		InnerAlloc allocator;

		Node* head{ nullptr };
		Node* tail{ nullptr };

		std::mutex mt;

	};

};