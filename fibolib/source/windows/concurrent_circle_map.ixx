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
		using ConVec = Concurrency::concurrent_vector<mapped_type>;
		using ConMap = Concurrency::concurrent_unordered_map<key_type, size_type>;

	public:
		CircleMap() noexcept(std::is_nothrow_constructible_v<ConVec>and std::is_nothrow_constructible_v<ConMap>) = default;
		~CircleMap() noexcept = default;

		// copyable
		CircleMap(CircleMap const& other) :
			mLastPos{ other.mLastPos.load(std::memory_order_relaxed) },
			mData{ other.mData },
			mKeys{other.mKeys }
		{}

		CircleMap& operator=(CircleMap const& other)
		{
			auto tmp(other);
			*this = std::move(tmp);
			return *this;
		}

		// movable
		CircleMap(CircleMap&& other) noexcept:
			mLastPos{ other.mLastPos.load(std::memory_order_relaxed) },
			mData{ std::exchange(other.mData,  ConVec{}) },
			mKeys{ std::exchange(other.mKeys, ConMap{}) }
		{
			other.mLastPos.store(0, std::memory_order_relaxed);
		}

		CircleMap& operator=(CircleMap&& other) noexcept
		{
			if (this not_eq &other) {
				mLastPos.store(other.mLastPos.load(std::memory_order_relaxed));
				mData = std::exchange(other.mData, ConVec{});
				mKeys = std::exchange(other.mKeys, ConMap{});
				other.mLastPos.store(0, std::memory_order_relaxed);
			}
			return *this;
		}

		constexpr auto maxSize() const noexcept { return N; }
		constexpr auto size() const noexcept { return mKeys.size(); }

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
			return opos ? ReturnType{ mData[*opos] } : ReturnType{ std::nullopt };
		}

		template<class Condition>
		auto find_if(Condition cond) const
		{
			using ReturnType = std::optional<mapped_type>;

			for (auto const& [k, p] : mKeys) {
				if (cond(k, mData[p])) {
					return ReturnType{ mData[p] };
				}
			}
			return ReturnType{ std::nullopt };
		}

		mapped_type& operator[](key_type const& key)
		{
			auto opos = findInternal(key);
			if (opos) {
				return mData[*opos];
			}

			// Not found => create new pair
			auto pos = nextPos();
			mKeys[key] = static_cast<size_type>(pos);
			return mData[pos];
		}

		/// <summary>
		/// unsafe_earase maybe race condition
		/// </summary>
		/// <param name="key"></param>
		/// <returns></returns>
		constexpr auto unsafe_erase(key_type const& key)
		{
			return mKeys.unsafe_erase(key);
		}

	private:
		auto findInternal(key_type const& key) const
		{
			using ReturnType = std::optional<size_type>;
			auto found = mKeys.find(key);
			if (std::cend(mKeys) == found) {
				return ReturnType{ std::nullopt };
			}
			return ReturnType{ found->second };
		}

		constexpr size_t nextPos() noexcept { return mLastPos++ % N; }

	private:
		std::atomic_size_t mLastPos{0};
		ConVec mData{ N };
		ConMap mKeys; // Doesn't have reserve()
	};

	/**********************************************************************************************/
}
