module;

#include <concurrent_vector.h>
#include <atomic>
#include <execution>
#include <optional>
#include <unordered_map> //++ todo remove

export module Fibo.ConcurrentCircleMap;

namespace fibo::Con
{
	export template<
		class KEY, 
		class T, 
		size_t N = 3,
		class KeyEqual = std::equal_to<KEY>>
	class CircleMap
	{
		struct KeyMapping
		{
			KEY		key_{};
			size_t	pos_{0};
		};

	public:
		using key_type		= KEY;
		using mapped_type	= T;
		using key_equal		= KeyEqual;
		using const_iterator_key	= typename Concurrency::concurrent_vector<KeyMapping>::const_iterator;
		using const_iterator		= typename Concurrency::concurrent_vector<T>::const_iterator;

	public:
		//++ TODO does compliler generate move ??? => YES

		auto find(key_type const& key) const
		{ 
			using ReturnType = std::optional<std::reference_wrapper<mapped_type const>>;
			auto found = findInternal(key);
			if (std::end(keys_) != found) {
				return ReturnType{ vec_[found->pos_] };
			}
			return ReturnType{ std::nullopt };
		}

		auto find(key_type const& key)
		{
			using ReturnType = std::optional<mapped_type>;
			auto found = findInternal(key);
			if (std::end(keys_) != found) {
				return ReturnType{ vec_[found->pos_] };
			}
			return ReturnType{ std::nullopt };
		}

		mapped_type& operator[](key_type const& key)
		{
			auto found = findInternal(key);
			if (std::end(keys_) != found) {
				return vec_[found->pos_];
			}

			// Not found => create new pair
			auto pos = nextPos();
			keys_[pos] = KeyMapping{ .key_ = key, .pos_ = pos };
			return vec_[pos];
		}

		mapped_type& operator[](key_type&& key)
		{
			auto found = findInternal(key);
			if (std::end(keys_) != found) {
				return vec_[found->pos_];
			}

			// Not found => create new pair
			auto pos = nextPos();
			keys_[pos] = KeyMapping{ .key_ = std::move(key), .pos_ = pos };
			return vec_[pos];
		}

	private:
		const_iterator_key findInternal(key_type const& key) const
		{
			return std::find_if(
				std::execution::seq,
				std::cbegin(keys_),
				std::cend(keys_),
				[&key](auto const& item) {
					return key_equal{}(key, item.key_); //++ TODO create key_equal
				});
		}

		constexpr size_t nextPos() noexcept { return pos_++ % N; }

	private:
		std::atomic_size_t pos_{0};
		Concurrency::concurrent_vector<T> vec_{ N };
		Concurrency::concurrent_vector<KeyMapping> keys_{ N };
	};

	/**********************************************************************************************/
}
