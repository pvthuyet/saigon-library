#include "pch.h"
#include "test_string_utils.h"
#include "string_utils.h"
#include "nlohmann\json.hpp"
#include <fstream>
#include <chrono>

#define K_SPLIT				"split"
#define K_TCID				"TCID"
#define K_INPUT_STRING		"input_string"

using json = nlohmann::json;
using fibo_string = std::string;

struct SplitData
{
	std::string tcid;
	std::string inputString;
};

decltype(auto) readJsonData(std::string_view sPath)
{
	std::vector<SplitData> data;
	std::ifstream ifs(sPath.data());
	json js = json::parse(ifs);
	for (auto& e : js[K_SPLIT])
	{
		SplitData elm;

		std::string str = e[K_TCID];
		elm.tcid = fibo_string(str.begin(), str.end());

		str = e[K_INPUT_STRING];
		elm.inputString = fibo_string(str.begin(), str.end());

		data.push_back(elm);
	}
	return data;
}

void testSplit()
{
	try
	{
		auto input = readJsonData("test_data\\string_utils.json");
		for (const auto& e : input)
		{
			fibo_string str(e.inputString.begin(), e.inputString.end());
			std::string token {"\\s+"};
			auto start = std::chrono::system_clock::now();
			auto result = fibo::StringUtils::split(str, token);
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> diff = end - start;
			std::cout << "time to split() " << result.size() << " => " << std::setw(9) << diff.count() << " s\n";
		}
	}
	catch (const std::exception & ex)
	{
		std::cout << ex.what() << std::endl;
		EXPECT_TRUE(false);
	}
}