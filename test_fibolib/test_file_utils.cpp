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

template<typename T>
std::vector<FileNameInformation<T>> readJsonData(std::string fpath)
{
	std::vector<FileNameInformation<T>> data;
	std::ifstream ifs(fpath);
	json js = json::parse(ifs);
	std::string str;

	for (auto& e : js[K_PARSE_FILE_NAME])
	{
		FileNameInformation<T> info;
		str = e[K_FULL_PATH];
		info.mFullPath		= T(str.begin(), str.end());

		str = e[K_PARENT];
		info.mParentPath	= T(str.begin(), str.end());

		str = e[K_ROOT];
		info.mRootPath		= T(str.begin(), str.end());

		str = e[K_NAME];
		info.mFileName		= T(str.begin(), str.end());

		str = e[K_STEM];
		info.mStem			= T(str.begin(), str.end());

		str = e[K_EXTENSION];
		info.mExtension		= T(str.begin(), str.end());
		data.push_back(std::move(info));
	}
	return data;
}

bool testParseFileName()
{
	// test wstring
	auto input1 = readJsonData<std::wstring>("test_data\\file_utils.json");
	for (const auto& pa : input1)
	{
		auto info = parseFileName(pa.mFullPath);
		EXPECT_EQ(info.mFullPath, pa.mFullPath);
		EXPECT_EQ(info.mParentPath, pa.mParentPath);
		EXPECT_EQ(info.mRootPath, pa.mRootPath);
		EXPECT_EQ(info.mFileName, pa.mFileName);
		EXPECT_EQ(info.mStem, pa.mStem);
		EXPECT_EQ(info.mExtension, pa.mExtension);
	}

	// test string
	auto input2 = readJsonData<std::string>("test_data\\file_utils.json");
	for (const auto& pa : input2)
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