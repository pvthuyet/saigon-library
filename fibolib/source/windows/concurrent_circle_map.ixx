module;

#include <concurrent_vector.h>
#include <atomic>
#include <execution>
#include <optional>
#include <limits>
#include <unordered_map> //++ todo remove

export module Fibo.ConcurrentCircleMap;

namespace fibo::Con
{
	export template<
		class KEY, 
		class T, 
		unsigned int N = std::numeric_limits<unsigned int>::max(),
		class KeyEqual = std::equal_to<KEY>>
	class CircleMap
	{
		struct KeyMapping
		{
			static constexpr size_t InvalidPos = N + 1;
			KEY		key_{};
			size_t	pos_{ InvalidPos };
			constexpr explicit operator bool() const noexcept { return InvalidPos != pos_; }
		};

	public:
		using key_type		= KEY;
		using mapped_type	= T;
		//using key_equal		= KeyEqual;
		//using const_iterator_key	= typename Concurrency::concurrent_vector<KeyMapping>::const_iterator;
		//using const_iterator		= typename Concurrency::concurrent_vector<T>::const_iterator;

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
		auto find(key_type const& key)
		{
			using ReturnType = std::optional<mapped_type>;
			auto found = findInternal(key);
			if (found) {
				return ReturnType{ vec_[found->get().pos_] };
			}
			return ReturnType{ std::nullopt };
		}

		mapped_type& operator[](key_type const& key)
		{
			auto found = findInternal(key);
			if (found) {
				return vec_[found->get().pos_];
			}

			// Not found => create new pair
			auto pos = nextPos();
			keys_[pos] = KeyMapping{ .key_ = key, .pos_ = pos };
			return vec_[pos];
		}

		mapped_type& operator[](key_type&& key)
		{
			auto found = findInternal(key);
			if (found) {
				return vec_[found->get().pos_];
			}

			// Not found => create new pair
			auto pos = nextPos();
			keys_[pos] = KeyMapping{ .key_ = std::move(key), .pos_ = pos };
			return vec_[pos];
		}

	private:
		auto findInternal(key_type const& key) const
		{
			using ReturnType = std::optional<std::reference_wrapper<KeyMapping const>>;
			for (unsigned int i = 0; i < N; ++i) {
				auto const& info = keys_[i];
				if (!info) {
					return ReturnType{ std::nullopt };
				}

				if (key == info.key_) {
					return ReturnType{ keys_[i] };
				}
			}
			return ReturnType{ std::nullopt };
		}

		constexpr size_t nextPos() noexcept { return posElement_++ % N; }

	private:
		std::atomic_size_t posElement_{0};
		Concurrency::concurrent_vector<T> vec_{ N };
		Concurrency::concurrent_vector<KeyMapping> keys_{ N };
	};

	/**********************************************************************************************/
}
