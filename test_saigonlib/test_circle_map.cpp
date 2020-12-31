#include "pch.h"
#include <functional>

import Saigon.ConcurrentCircleMap;
constexpr unsigned int mapSz = 11;

using KeyType = std::string;
struct DataType 
{
	DataType() = default;
	explicit DataType(int v) : mValue{ v } {}
	int mValue{};
	constexpr auto operator<=>(DataType const&) const noexcept = default;
	operator bool() const
	{
		return 0 != mValue;
	}
};

using TestCirMap = saigon::con::circle_map<KeyType, DataType, mapSz>;


TestCirMap initMap(int sz = mapSz)
{
	using namespace std::string_literals;
	TestCirMap mp{};
	for (int i = 0; i < sz; ++i) {
		std::string key = "key "s + std::to_string(i);
		DataType v{ i };
		mp[key] = v;
	}
	return mp;
}

TEST(circle_map, constructor_fix_size)
{
	TestCirMap ciMap;
	EXPECT_EQ(mapSz, ciMap.size());
}

TEST(circle_map, insert_item_success)
{
	DataType v{1};
	KeyType key = "key 1";
	TestCirMap ciMap{};
	ciMap[key] = v;
	EXPECT_EQ(v, ciMap[key]);
}

TEST(circle_map, find_in_empty_circle_map)
{
	TestCirMap ciMap{};
	auto found = ciMap.find("key 1");
	EXPECT_TRUE(not found);
}


TEST(circle_map, find_available_item)
{
	DataType v{1};
	TestCirMap ciMap = initMap();
	auto found = ciMap.find("key 1");
	EXPECT_EQ(v, found);
}

TEST(circle_map, find_not_found_item)
{
	TestCirMap ciMap = initMap();
	auto found = ciMap.find("key -1");
	EXPECT_TRUE(not found);
}

TEST(circle_map, find_if_available_item)
{
	DataType v{10};
	TestCirMap ciMap = initMap();
	ciMap.next_available_item();
	auto found = ciMap.find_if([&v](auto const& val) {
		return val == v;
		});
	EXPECT_EQ(v, found);
}

TEST(circle_map, find_if_not_found_item)
{
	DataType v{ mapSz + 1 };
	TestCirMap ciMap = initMap();
	ciMap.next_available_item();
	ciMap.next_available_item();
	ciMap.next_available_item();
	auto found = ciMap.find_if([&v](auto const& val) {
		return val == v;
		});
	EXPECT_NE(v, found);
}

TEST(circle_map, rfind_if_not_found_item)
{
	DataType v{ mapSz + 1 };
	TestCirMap ciMap = initMap();
	ciMap.next_available_item();
	ciMap.next_available_item();
	ciMap.next_available_item();
	auto found = ciMap.rfind_if([&v](auto const& val) {
		return val == v;
		});
	EXPECT_NE(v, found);
}

TEST(circle_map, loop_all_if_not_found_item)
{
	DataType v{ mapSz + 1 };
	TestCirMap ciMap = initMap();
	ciMap.next_available_item();
	ciMap.next_available_item();
	ciMap.next_available_item();
	std::vector<std::reference_wrapper<const DataType>> vec;
	ciMap.loop_all([&v, &vec](auto const& val) {
		if (val == v) {
			vec.push_back(val);
		}
		});
	EXPECT_TRUE(0==vec.size());
}

TEST(circle_map, rloop_all_if_not_found_item)
{
	DataType v{ mapSz + 1 };
	TestCirMap ciMap = initMap();
	ciMap.next_available_item();
	ciMap.next_available_item();
	ciMap.next_available_item();
	std::vector<std::reference_wrapper<const DataType>> vec;
	ciMap.rloop_all([&v, &vec](auto const& val) {
		if (val == v) {
			vec.push_back(val);
		}
		});
	EXPECT_TRUE(0 == vec.size());
}

TEST(circle_map, erase_item)
{
	DataType v{ 1 };
	TestCirMap ciMap = initMap();
	auto found = ciMap.find("key 1");
	EXPECT_EQ(v, found);

	// erase
	ciMap.erase("key 1");
	auto found2 = ciMap.find("key 1");
	EXPECT_TRUE(not found2);
}