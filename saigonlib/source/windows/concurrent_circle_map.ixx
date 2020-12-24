module;

#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>
#include <atomic>
#include <execution>
#include <optional>
#include <limits>
#include <algorithm>

export module Saigon.ConcurrentCircleMap;

namespace saigon::con
{
	//++ TODO: require default ctor T
	export template<
		class KEY, 
		class T, 
		unsigned int N = std::numeric_limits<unsigned int>::max() - 1> requires (N < std::numeric_limits<unsigned int>::max())
	class circle_map final
	{
		static constexpr unsigned int INVALID_INDEX = N + 1;
	public:
		using key_type		= KEY;
		using mapped_type	= T;
		using size_type		= unsigned int;
		using con_vec = Concurrency::concurrent_vector<mapped_type>;
		using con_map = Concurrency::concurrent_unordered_map<key_type, size_type>;

		using reference			= mapped_type&;
		using const_reference	= const mapped_type&;
		using optional_ref		= std::optional <std::reference_wrapper<mapped_type>>;
		using optional_cref		= std::optional <std::reference_wrapper<const mapped_type>>;

	public:
		circle_map() noexcept(std::is_nothrow_constructible_v<con_vec> and std::is_nothrow_constructible_v<con_map>) = default;
		~circle_map() noexcept = default;

		// copyable
		circle_map(circle_map const& other) :
			mPushIndex{ other.mPushIndex.load(std::memory_order_relaxed) },
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
			mPushIndex{ other.mPushIndex.load(std::memory_order_relaxed) },
			mData{ std::exchange(other.mData,  con_vec{}) },
			mKeys{ std::exchange(other.mKeys, con_map{}) }
		{
			other.mPushIndex.store(0, std::memory_order_relaxed);
		}

		circle_map& operator=(circle_map&& other) noexcept
		{
			if (this not_eq &other) {
				mPushIndex.store(other.mPushIndex.load(std::memory_order_relaxed));
				mData = std::exchange(other.mData, con_vec{});
				mKeys = std::exchange(other.mKeys, con_map{});
				other.mPushIndex.store(0, std::memory_order_relaxed);
			}
			return *this;
		}

		constexpr auto max_size() const noexcept { return N; }
		auto size() const noexcept { return mKeys.size(); }

		optional_cref find(key_type const& key) const
		{
			auto pos = find_internal(key);
			return (INVALID_INDEX == pos) ? optional_cref{ std::nullopt } : optional_cref{ mData[pos] };
		}

		template<class Predicate>
		optional_cref find_if(Predicate pre) const
		{
			//auto fnd = std::find_if(mData.cbegin(), mData.cend(), pre);
			//auto found = std::ranges::find_if(mData, pre);

			//++ TODO unknow error 
			//if (mData.cend() != found) {
				//return ReturnType{ *fnd };
			//}

			return optional_cref{ std::nullopt };
		}

		reference operator[](key_type const& key)
		{
			auto pos = find_internal(key);
			if (INVALID_INDEX != pos) {
				return mData[pos];
			}

			// Not found => create new pair
			auto idx = next_push_index();
			mKeys[key] = idx;
			return mData[idx];
		}

		void erase(key_type const& key)
		{
			auto found = mKeys.find(key);

			// not found or invlid
			if (std::cend(mKeys) == found  or INVALID_INDEX == found->second) {
				return;
			}

			// reset data
			mData[found->second] = mapped_type{};
			found->second = INVALID_INDEX;
		}

	private:
		size_type find_internal(key_type const& key) const
		{
			auto found = mKeys.find(key);
			if (std::cend(mKeys) == found or INVALID_INDEX == found->second) {
				return INVALID_INDEX;
			}
			return found->second;
		}

		constexpr size_type next_push_index() noexcept { return mPushIndex++ % N; }

	private:
		std::atomic<size_type> mPushIndex{0};
		con_vec mData{ N };
		con_map mKeys; // Doesn't have reserve()
	};

	/**********************************************************************************************/
}
