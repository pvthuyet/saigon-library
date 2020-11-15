module;

#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>
#include <atomic>
#include <execution>
#include <optional>
#include <limits>
#include <unordered_map> //++ todo remove
#include <map>

export module Fibo.ConcurrentCircleMap;

namespace fibo::Con
{
	export template<
		class KEY, 
		class T, 
		unsigned int N = std::numeric_limits<unsigned int>::max()>
	class CircleMap
	{
	public:
		using key_type		= KEY;
		using mapped_type	= T;
		using size_type		= unsigned int;

	public:
		//++ TODO does compliler generate move ??? => YES

		constexpr size_t size() const noexcept { return N; }

		/// <summary>
		/// Find the value in map by key
		/// </summary>
		/// <param name="key"></param>
		/// <returns>Must return by value of T and wrapped by std::optional. 
		/// Can't return reference because of multiple threads
		/// </returns>
		auto find(key_type const& key) const
		{
			using ReturnType = std::optional<mapped_type>;
			auto opos = findInternal(key);
			return opos ? ReturnType{ data_[*opos] } : ReturnType{ std::nullopt };
		}

		template<class Condition>
		auto find_if(Condition cond) const
		{
			using ReturnType = std::optional<mapped_type>;

			for (auto const& [k, p] : keys_) {
				if (cond(k, data_[p])) {
					return ReturnType{ data_[p] };
				}
			}
			return ReturnType{ std::nullopt };
		}

		mapped_type& operator[](key_type const& key)
		{
			auto opos = findInternal(key);
			if (opos) {
				return data_[*opos];
			}

			// Not found => create new pair
			auto pos = nextPos();
			keys_[key] = pos;
			return data_[pos];
		}

		constexpr size_type erase(key_type const& key)
		{
			return 1;
		}

	private:
		auto findInternal(key_type const& key) const
		{
			using ReturnType = std::optional<size_type>;
			auto found = keys_.find(key);
			if (std::cend(keys_) == found) {
				return ReturnType{ std::nullopt };
			}
			return ReturnType{ found->second };
		}

		constexpr size_t nextPos() noexcept { return lastPos_++ % N; }

	private:
		std::atomic_size_t lastPos_{0};
		Concurrency::concurrent_vector<mapped_type> data_{ N };
		Concurrency::concurrent_unordered_map<key_type, size_type> keys_;
	};

	/**********************************************************************************************/
}
