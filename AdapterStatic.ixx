module;
#include <cassert>

export module fovere.Adapter.Static;

import hfog.Core;

template<typename Arr>
class StaticAlloc
{
public:
	StaticAlloc() = default;

	StaticAlloc(const StaticAlloc&) = delete;
	StaticAlloc& operator=(const StaticAlloc&) = delete;

	StaticAlloc(StaticAlloc&&) = default;
	StaticAlloc& operator=(StaticAlloc&&) = default;

	Arr::TAlloc staticAlloc;

};

export namespace fovere::Adapter
{

	template<typename Arr>
	class Static : public StaticAlloc<Arr>, public Arr
	{
	public:
		Static(const Static&) = delete;
		Static& operator=(const Static&) = delete;

		Static(Static&&) = default;
		Static& operator=(Static&&) = default;
		
		Static():
			Arr(&this->staticAlloc)
		{}
	private:
	};

};