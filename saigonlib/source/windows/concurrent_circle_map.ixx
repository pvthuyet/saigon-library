module;

#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>
#include <atomic>
#include <execution>
#include <optional>
#include <limits>

export module Saigon.ConcurrentCircleMap;

namespace saigon::Con
{
	export template<
		class KEY, 
		class T, 
		unsigned int N = std::numeric_limits<unsigned int>::max()>
	class circle_map final
	{
	public:
		using key_type		= KEY;
		using mapped_type	= T;
		using size_type		= unsigned int;
		using con_vec = Concurrency::concurrent_vector<mapped_type>;
		using con_map = Concurrency::concurrent_unordered_map<key_type, size_type>;

	public:
		circle_map() noexcept(std::is_nothrow_constructible_v<con_vec>and std::is_nothrow_constructible_v<con_map>) = default;
		~circle_map() noexcept = default;

		// copyable
		circle_map(circle_map const& other) :
			mLastPos{ other.mLastPos.load(std::memory_order_relaxed) },
			mData{ other.mData },
			mKeys{other.mKeys }
		{}

		circle_map& operator=(circle_map const& other)
		{
			auto tmp(other);
			*this = std::move(tmp);
			return *this;
		}

		// movable
		circle_map(circle_map&& other) noexcept:
			mLastPos{ other.mLastPos.load(std::memory_order_relaxed) },
			mData{ std::exchange(other.mData,  con_vec{}) },
			mKeys{ std::exchange(other.mKeys, con_map{}) }
		{
			other.mLastPos.store(0, std::memory_order_relaxed);
		}

		circle_map& operator=(circle_map&& other) noexcept
		{
			if (this not_eq &other) {
				mLastPos.store(other.mLastPos.load(std::memory_order_relaxed));
				mData = std::exchange(other.mData, con_vec{});
				mKeys = std::exchange(other.mKeys, con_map{});
				other.mLastPos.store(0, std::memory_order_relaxed);
			}
			return *this;
		}

		constexpr auto max_size() const noexcept { return N; }
		constexpr auto size() const noexcept { return mKeys.size(); }

		auto find(key_type const& key) const
		{
			using ReturnType = std::optional<mapped_type>;
			auto opos = find_internal(key);
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
			auto opos = find_internal(key);
			if (opos) {
				return mData[*opos];
			}

			// Not found => create new pair
			auto pos = next_pos();
			mKeys[key] = static_cast<size_type>(pos);
			return mData[pos];
		}

		constexpr auto unsafe_erase(key_type const& key)
		{
			return mKeys.unsafe_erase(key);
		}

	private:
		auto find_internal(key_type const& key) const
		{
			using ReturnType = std::optional<size_type>;
			auto found = mKeys.find(key);
			if (std::cend(mKeys) == found) {
				return ReturnType{ std::nullopt };
			}
			return ReturnType{ found->second };
		}

		constexpr size_t next_pos() noexcept { return mLastPos++ % N; }

	private:
		std::atomic_size_t mLastPos{0};
		con_vec mData{ N };
		con_map mKeys; // Doesn't have reserve()
	};

	/**********************************************************************************************/
}
