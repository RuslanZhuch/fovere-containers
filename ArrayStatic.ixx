module;
#include <cassert>

export module fovere.Array.Static;
import fovere.Array.DynamicCompact;

import hfog.Algorithms.Stack;
import hfog.Sources.Local;

import hfog.Core;

template<size_t capacity, typename T>
using source_t = hfog::Sources::Local<capacity * sizeof(T)>;

template<size_t capacity, typename T>
using allocator_t = hfog::Algorithms::Stack<source_t<capacity, T>, sizeof(T), hfog::MemoryUtils::Align<sizeof(T)>>;

template<size_t capacity, typename T>
using base_t = fovere::Array::DynamicCompact<allocator_t<capacity, T>, T>;

export namespace fovere::Array
{

	template<size_t capacity, typename T>
	class Static : public base_t<capacity, T>
	{
	public:
		Static(const Static&) = delete;
		Static& operator=(const Static&) = delete;

		Static(Static&&) = default;
		Static& operator=(Static&&) = default;
		Static():
			base_t<capacity, T>(&staticAlloc)
		{}
	private:
		allocator_t<capacity, T> staticAlloc;
	};

};