#include "pch.h"
#include "test_file_utils.h"
#include "file_utils.h"
#include "nlohmann\json.hpp"
#include <fstream>

#define	K_PARSE_FILE_NAME		"parse_file_name"
#define	K_FULL_PATH		"full_path"
#define	K_PARENT		"parent"
#define	K_ROOT			"root"
#define	K_NAME			"name"
#define	K_STEM			"stem"
#define	K_EXTENSION		"extension"

using namespace fibo::FileUtils;
using json = nlohmann::json;

std::vector<FileNameInformation<std::string>> readJsonData(std::string fpath)
{
	std::vector<FileNameInformation<std::string>> data;
	std::ifstream ifs("test_data\\file_utils.json");
	json js = json::parse(ifs);

	for (auto& e : js[K_PARSE_FILE_NAME])
	{
		FileNameInformation<std::string> info;
		info.mFullPath		= e[K_FULL_PATH];
		info.mParentPath	= e[K_PARENT];
		info.mRootPath		= e[K_ROOT];
		info.mFileName		= e[K_NAME];
		info.mStem			= e[K_STEM];
		info.mExtension		= e[K_EXTENSION];
		data.push_back(std::move(info));
	}
	return data;
}

bool testParseFileName()
{
	auto input = readJsonData("test_data\\file_utils.json");
	for (const auto& pa : input)
	{
		auto info = parseFileName(pa.mFullPath);
		EXPECT_EQ(info.mFullPath, pa.mFullPath);
		EXPECT_EQ(info.mParentPath, pa.mParentPath);
		EXPECT_EQ(info.mRootPath, pa.mRootPath);
		EXPECT_EQ(info.mFileName, pa.mFileName);
		EXPECT_EQ(info.mStem, pa.mStem);
		EXPECT_EQ(info.mExtension, pa.mExtension);
	}
	return true;
}