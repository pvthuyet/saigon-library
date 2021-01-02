module;

#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>
#include <atomic>
#include <execution>
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
		static_assert(N > 0, "Map size must be greater than 0");
		static_assert(N < UINT_MAX, "Map size must be less than UINT_MAX");

	public:
		using key_type = KEY;
		using mapped_type = T;
		using size_type = unsigned int;
		using con_vec = Concurrency::concurrent_vector<mapped_type>;
		using con_map = Concurrency::concurrent_unordered_map<key_type, size_type>;
		using reference = mapped_type&;
		using const_reference = const mapped_type&;

	public:
		circle_map() noexcept(std::is_nothrow_constructible_v<con_vec>&& std::is_nothrow_constructible_v<con_map>) = default;
		~circle_map() noexcept = default;

		// copyable
		circle_map(circle_map const& other) :
			mEmpty{ other.mEmpty.load(std::memory_order_relaxed) },
			mPushIndex{ other.mPushIndex.load(std::memory_order_relaxed) },
			mPopIndex{ other.mPopIndex.load(std::memory_order_relaxed) },
			mData{ other.mData },
			mKeys{ other.mKeys }
		{}

		circle_map& operator=(circle_map const& other)
		{
			auto tmp(other);
			*this = std::move(tmp);
			return *this;
		}

		// movable
		circle_map(circle_map&& other) noexcept :
			mEmpty{ other.mEmpty.load(std::memory_order_relaxed) },
			mPushIndex{ other.mPushIndex.load(std::memory_order_relaxed) },
			mPopIndex{ other.mPopIndex.load(std::memory_order_relaxed) },
			mData{ std::exchange(other.mData,  con_vec{}) },
			mKeys{ std::exchange(other.mKeys, con_map{}) }
		{
			// reset other atomic
			other.mEmpty.store(true, std::memory_order_relaxed);
			other.mPushIndex.store(0, std::memory_order_relaxed);
			other.mPopIndex.store(0, std::memory_order_relaxed);
		}

		circle_map& operator=(circle_map&& other) noexcept
		{
			if (this != &other) {
				mEmpty.store(other.mEmpty.load(std::memory_order_relaxed));
				mPushIndex.store(other.mPushIndex.load(std::memory_order_relaxed));
				mPopIndex.store(other.mPopIndex.load(std::memory_order_relaxed));
				mData = std::exchange(other.mData, con_vec{});
				mKeys = std::exchange(other.mKeys, con_map{});

				// reset other atomic
				other.mEmpty.store(true, std::memory_order_relaxed);
				other.mPushIndex.store(0, std::memory_order_relaxed);
				other.mPopIndex.store(0, std::memory_order_relaxed);
			}
			return *this;
		}

		constexpr size_type size() const noexcept
		{
			return N;
		}

		bool empty() const noexcept
		{
			return mEmpty.load(std::memory_order_relaxed);
		}

		const_reference find(key_type const& key) const
		{
			auto pos = find_internal(key);
			return (INVALID_INDEX != pos) ? mData[pos] : EMPTY_ITEM;
		}

		template<class Predicate>
		const_reference find_if(Predicate pre) const
		{
			// empty map
			if (empty()) {
				return EMPTY_ITEM;
			}

			// not empty
			bool found = false;
			size_type pos = mPopIndex.load(std::memory_order_relaxed);
			for (size_type i = 0; i < N; ++i) { // circle search
				size_type idx = (pos + i) % N;
				const auto& item = mData[idx];
				if (item && pre(item)) {
					found = true;
					pos = idx;
					break;
				}
			}

			return found ? mData[pos] : EMPTY_ITEM;
		}

		template<class Predicate>
		const_reference rfind_if(Predicate pre) const
		{
			// empty map
			if (empty()) {
				return EMPTY_ITEM;
			}

			// not empty
			bool found = false;
			size_type pos = mPopIndex.load(std::memory_order_relaxed) + 1; // start with current pos
			for (size_type i = 0; i < N; ++i) { // circle search
				const auto& item = mData[--pos];
				if (item && pre(item)) {
					found = true;
					break;
				}
				if (0 == pos) pos = N;
			}

			return found ? mData[pos] : EMPTY_ITEM;
		}

		template<class Func>
		void loop_all(Func invoke) const
		{
			// empty map
			if (empty()) {
				return;
			}
			size_type pos = mPopIndex.load(std::memory_order_relaxed);
			for (size_type i = 0; i < N; ++i) { // circle search
				size_type idx = (pos + i) % N;
				const auto& item = mData[idx];
				if (item) {
					invoke(item);
				}
			}
		}

		template<class Func>
		void rloop_all(Func invoke) const
		{
			// empty map
			if (empty()) {
				return;
			}
			size_type pos = mPopIndex.load(std::memory_order_relaxed) + 1;
			for (size_type i = 0; i < N; ++i) { // circle search
				const auto& item = mData[--pos];
				if (item) {
					invoke(item);
				}
				if (0 == pos) pos = N;
			}
		}

		reference operator[](key_type const& key)
		{
			//++ TODO
			// Safe reset mKeys data
			// reason: this called by push thread, always set empty is false
			// pop thread update empty is true
			// Whenever the empty is true, that mean no thread touch data
			// So, safe to reset mKeys
			//if (empty()) {
			//	mKeys.clear();
			//}

			// This function is considered as add item to map
			// Whenever it is called should change the empty state
			updateEmpty(false);

			size_type pos = find_internal(key);
			if (INVALID_INDEX != pos) {
				return mData[pos];
			}

			// Not found => create new pair
			size_type curIndex = next_push_index();
			mKeys[key] = curIndex;
			return mData[curIndex];
		}

		void erase(key_type const& key)
		{
			auto found = mKeys.find(key);

			// Not valid
			if (std::cend(mKeys) == found || INVALID_INDEX == found->second) {
				return;
			}

			// reset data
			mData[found->second] = mapped_type{};
			found->second = INVALID_INDEX;
		}

		const_reference front() const
		{
			return mData[get_pop_index()];
		}

		size_type next_available_item()
		{
			// Empty data
			if (empty()) {
				return mPopIndex.load(std::memory_order_relaxed);
			}

			size_type old = mPopIndex.load(std::memory_order_relaxed);
			size_type next = old;
			for (size_type i = 0; i < N; ++i) { // Circle search
				next = (next + 1) % N;
				if (mData[next]) {
					break;
				}
			}

			// No more data
			if (next == old) {
				// Mark as empty map
				updateEmpty(true);
				// The 'old' is already processed => should ignore it
				next = (old + 1) % N;
			}

			// avaible item => update to atomic
			while (!mPopIndex.compare_exchange_weak(old, next, std::memory_order_release, std::memory_order_relaxed)) {}
			return next;
		}

	private:
		size_type find_internal(key_type const& key) const
		{
			// Empty map
			if (empty()) {
				return INVALID_INDEX;
			}

			// not empty
			auto found = mKeys.find(key);
			if (std::cend(mKeys) == found || INVALID_INDEX == found->second) {
				return INVALID_INDEX;
			}
			return found->second;
		}

		size_type next_push_index() noexcept
		{
			return mPushIndex++ % N;
		}

		size_type get_pop_index() const noexcept
		{
			return mPopIndex.load(std::memory_order_relaxed) % N;
		}

		bool updateEmpty(bool val)
		{
			bool old = mEmpty.load(std::memory_order_relaxed);
			while (!mEmpty.compare_exchange_weak(old, val, std::memory_order_release, std::memory_order_relaxed)) {}
			return old;
		}

	private:
		std::atomic_bool mEmpty{ true };
		std::atomic<size_type> mPushIndex{ 0u };
		std::atomic<size_type> mPopIndex{ 0u };
		con_vec mData{ N };
		con_map mKeys; // Doesn't have reserve()
		const unsigned int	INVALID_INDEX = N + 1;
		const mapped_type	EMPTY_ITEM{};
	};

	/**********************************************************************************************/
}
