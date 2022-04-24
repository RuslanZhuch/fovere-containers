module;
#include <cassert>
#include <optional>

export module fovere.Map.Simple;

import fovere.Utils;
import hfog.Core;

import fovere.Array.Universal;

export namespace fovere::Map
{

	template <typename TKey, typename TVal, hfog::CtAllocator Alloc>
	class Simple
	{

	public:
		Simple(Alloc* alloc) 
			: 
			allocator(alloc),
			keys(allocator),
			vals(allocator)
		{	}

		void insert(const TKey& key, const TVal& val)
		{
			
			if (const auto id{ this->findIndex(key) }; id != invalidIndex)
			{
				this->vals[id] = val;
				return;
			}

			const auto keyId{ this->keys.append(key) };
			assert(keyId != invalidIndex);
			const auto valId{ this->vals.append(val) };
			assert(valId != invalidIndex);

			++this->localLen;

		}

		[[nodiscard]] auto& operator[](const TKey& key) noexcept
		{

			if (const auto id{ this->findIndex(key) }; id != invalidIndex)
			{
				return this->vals[id];
			}

			const auto keyId{ this->keys.append(key) };
			assert(keyId != invalidIndex);

			++this->localLen;

			const auto valId{ this->vals.append(TVal()) };
			assert(valId == keyId);

			return this->vals[keyId];

		}

		[[nodiscard]] constexpr const auto& operator[](const TKey& key) const noexcept
		{
			const auto id{ this->findIndex(key) };
			assert(id != invalidIndex);
			return this->vals[id];
		}

		void clear() noexcept
		{

			this->keys.clear();
			this->vals.clear();

			this->localLen = 0;

		}

		void remove(const TKey& key) noexcept
		{
			const auto id{ this->findIndex(key) };
			if (id == invalidIndex)
				return;
			this->keys.remove(id);
			this->vals.remove(id);

			--this->localLen;
		}

		[[nodiscard]] constexpr std::optional<TVal> find(const TKey& key) const noexcept
		{
			const auto id{ this->findIndex(key) };
			if (id == invalidIndex)
				return std::nullopt;
			return this->vals[id];
		}

		[[nodiscard]] constexpr auto getLen() const noexcept
		{
			return this->localLen;
		}

	private:
		[[nodiscard]] constexpr auto getElement(const TKey& key) noexcept
		{
			const auto id{ this->findIndex(key) };
			assert(id != invalidIndex);
			return this->vals[id];
		}

		[[nodiscard]] size_t findIndex(const TKey& key) const noexcept
		{

			for (size_t id{ 0 }; const auto & k : this->keys)
			{
				if ((k == key)/* && (this->existing[id] == true)*/)
					return id;
				++id;
			}

			return invalidIndex;

		}

	private:
		Alloc* allocator{ nullptr };

		size_t localLen{ 0 };

		Array::Universal<Alloc, TKey> keys;
		Array::Universal<Alloc, TVal> vals;

	};

};