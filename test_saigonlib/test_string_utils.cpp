#include "pch.h"
#include <ranges>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

import Saigon.StringUtils;

TEST(string_utils, split)
{
	auto vec = saigon::StringUtils::split("--hello--world-from-split-",
		[](auto const ch) { 
		return ch == '-'; 
	});
	EXPECT_EQ(4, vec.size());
}