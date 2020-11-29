#include "pch.h"

import Fibo.ConcurrentCircleMap;
constexpr unsigned int mapSz = 1024;

using KeyType = std::string;
struct DataType 
{
	DataType() = default;
	explicit DataType(int v) : value_{ v } {}
	int value_{};
	constexpr auto operator<=>(DataType const&) const noexcept = default;
};

using TestCirMap = fibo::Con::circle_map<KeyType, DataType, mapSz>;

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
	EXPECT_EQ(v, *found);
}

TEST(circle_map, find_not_found_item)
{
	TestCirMap ciMap = initMap();
	auto found = ciMap.find("key -1");
	EXPECT_TRUE(not found);
}

TEST(circle_map, find_if_available_item)
{
	DataType v{1};
	TestCirMap ciMap = initMap();
	auto found = ciMap.find_if([&](auto const& k, auto const& val) {
		return val == v;
		});
	EXPECT_EQ(v, *found);
}