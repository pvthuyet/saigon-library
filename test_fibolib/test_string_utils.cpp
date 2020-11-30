#include "pch.h"
#include <ranges>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

import Fibo.StringUtils;

TEST(string_utils, split)
{
	auto vec = fibo::StringUtils::split("hello   world from split",
		[sep = ' '](auto const c) { 
		return c == sep; 
	});
	EXPECT_EQ(4, vec.size());
}