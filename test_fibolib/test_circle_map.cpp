#include "pch.h"

import Fibo.ConcurrentCircleMap;
constexpr int mapSz = 100;

using KeyType = std::string;
struct DataType 
{
	int value_{};
	constexpr auto operator<=>(DataType const&) const noexcept = default;
};

TEST(CircleMap, constructor_fix_size)
{
	fibo::Con::CircleMap<KeyType, DataType, mapSz> ciMap{};
	EXPECT_EQ(mapSz, ciMap.size());
}

TEST(CircleMap, insert_item_by_reference_key)
{
	DataType v{};
	KeyType key = "key 1";
	fibo::Con::CircleMap<KeyType, DataType, mapSz> ciMap{};
	ciMap[key] = v;
	EXPECT_EQ(v, ciMap[key]);
}

TEST(CircleMap, insert_item_by_move_key)
{
	DataType v{};
	fibo::Con::CircleMap<KeyType, DataType, mapSz> ciMap{};
	ciMap["key 1"] = v;
	EXPECT_EQ(v, ciMap["key 1"]);
}

TEST(CircleMap, find_in_empty_circle_map)
{
	fibo::Con::CircleMap<KeyType, DataType, mapSz> ciMap{};
	auto found = ciMap.find("key 1");
	EXPECT_TRUE(not found);
}

TEST(CircleMap, find_available_item)
{
	DataType v{};
	fibo::Con::CircleMap<KeyType, DataType, mapSz> ciMap{};
	ciMap["key 1"] = v;
	auto found = ciMap.find("key 1");
	EXPECT_EQ(v, *found);
}

TEST(CircleMap, find_not_found_item)
{
	fibo::Con::CircleMap<KeyType, DataType, mapSz> ciMap{};
	ciMap["key 1"] = DataType{};
	auto found = ciMap.find("key 2");
	EXPECT_TRUE(not found);
}