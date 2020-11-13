module;

#include <concurrent_vector.h>
#include <atomic>
#include <execution>
#include <unordered_map> //++ todo remove

export module Fibo.ConcurrentBoundedMap;

namespace fibo::Con
{
	export template<
		class KEY, 
		class T, 
		class KeyEqual = std::equal_to<KEY>, 
		size_t N = 3>
	class BoundedMap
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

	public:
		//++ TODO does compliler generate move ??? => YES

		T& operator[](KEY const& key);
		T& operator[](KEY&& key);

		const T& find(KEY const &key) const;

	private:
		inline size_t nextPos() noexcept { return pos_++ % N; }
		T& pushBack(KEY const& key);

	private:
		std::atomic_size_t pos_{0};
		Concurrency::concurrent_vector<T> vec_{ N };
		Concurrency::concurrent_vector<KeyMapping> keys_{ N };
		const key_equal comparator_{};
	};

	/**********************************************************************************************/
	template<class KEY, class T, class KeyEqual, size_t N>
	const T& BoundedMap<KEY, T, KeyEqual, N>::find(KEY const& key) const
	{
		auto found = std::find_if(
			std::execution::seq,
			std::cbegin(keys_),
			std::cend(keys_),
			[&key, this](auto const &item) {
				return this->comparator_(key, item.key_);
			});

		if (std::cend(keys_) != found) {
			return vec_[found->pos_];
		}

		return vec_[0];
	}

	template<class KEY, class T, class KeyEqual, size_t N>
	T& BoundedMap<KEY, T, KeyEqual, N>::pushBack(KEY const& key)
	{
		auto slot = nextPos();
		vec_[slot] = v;
		keys_[slot] = KeyMapping{ .key_ = key, .pos_ = slot };
		return vec_[slot];
	}
}
