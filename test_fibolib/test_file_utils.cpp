#include "pch.h"
#include "test_file_utils.h"
#include "path_utils.h"
#include "nlohmann\json.hpp"
#include <fstream>

#define	K_PARSE_FILE_NAME		"parse_file_name"
#define	K_FULL_PATH				"full_path"
#define	K_ROOT_NAME				"root_name"
#define	K_ROOT_DIR				"root_dir"
#define	K_ROOT_PATH				"root_path"
#define	K_RELATIVE_PATH			"relative_path"
#define	K_PARENT_PATH			"parent_path"
#define	K_NAME					"name"
#define	K_STEM					"stem"
#define	K_EXTENSION				"extension"

#define K_ABSOLUTE_PATH			"absolute_path"
#define K_ORI_PATH				"ori_path"
#define K_EXP_PATH				"exp_path"

using namespace fibo::PathUtils;
using json = nlohmann::json;

struct AbsolutePathData
{
	std::wstring orgi;
	std::wstring exp;
};

decltype(auto) readJsonDataAbsolutePath(std::string_view sPath)
{
	std::vector<AbsolutePathData> data;
	std::ifstream ifs(sPath.data());
	json js = json::parse(ifs);
	for (auto& e : js[K_ABSOLUTE_PATH])
	{
		AbsolutePathData info;
		
		std::string str = e[K_ORI_PATH];
		info.orgi = std::wstring(str.begin(), str.end());

		str = e[K_EXP_PATH];
		info.exp  = std::wstring(str.begin(), str.end());
		data.push_back(info);
	}
	return data;
}

bool testAbsolutePath()
{
	try
	{
		auto input = readJsonDataAbsolutePath("test_data\\file_utils.json");
		for (const auto& e : input)
		{
			std::wstring str(e.orgi.begin(), e.orgi.end());
			auto ab = fibo::PathUtils::absolutePath(str);
			EXPECT_EQ(e.exp, ab);
		}
	}
	catch (const std::exception & ex)
	{
		std::cout << ex.what() << std::endl;
		EXPECT_TRUE(false);
	}
	return true;
}

struct FileNameDataTest
{
	std::wstring orgi;
	FileNameInformation info;
};

decltype(auto) readJsonDataParseFileName(std::string_view fpath)
{
	std::vector<FileNameDataTest> data;
	std::ifstream ifs(fpath.data());
	json js = json::parse(ifs);

	for (auto& e : js[K_PARSE_FILE_NAME])
	{
		FileNameDataTest inputData;

		std::string str = e[K_ORI_PATH];
		inputData.orgi = std::wstring(str.begin(), str.end());

		str = e[K_FULL_PATH];
		inputData.info.mFullPath	= std::wstring(str.begin(), str.end());

		str = e[K_ROOT_NAME];
		inputData.info.mRootName = std::wstring(str.begin(), str.end());

		str = e[K_ROOT_DIR];
		inputData.info.mRootDirectory = std::wstring(str.begin(), str.end());

		str = e[K_ROOT_PATH];
		inputData.info.mRootPath = std::wstring(str.begin(), str.end());

		str = e[K_RELATIVE_PATH];
		inputData.info.mRelativePath = std::wstring(str.begin(), str.end());

		str = e[K_PARENT_PATH];
		inputData.info.mParentPath = std::wstring(str.begin(), str.end());

		str = e[K_NAME];
		inputData.info.mFileName = std::wstring(str.begin(), str.end());

		str = e[K_STEM];
		inputData.info.mStem = std::wstring(str.begin(), str.end());

		str = e[K_EXTENSION];
		inputData.info.mExtension = std::wstring(str.begin(), str.end());
		data.push_back(std::move(inputData));
	}
	return data;
}

bool testParseFileName()
{
	try
	{
		// test wstring
		auto input1 = readJsonDataParseFileName("test_data\\file_utils.json");
		for (const auto& pa : input1)
		{
			auto info = parseFileName(pa.orgi);
			if (info)
			{
				EXPECT_EQ(info->mFullPath, pa.info.mFullPath);
				EXPECT_EQ(info->mRootName, pa.info.mRootName);
				EXPECT_EQ(info->mRootDirectory, pa.info.mRootDirectory);
				EXPECT_EQ(info->mRootPath, pa.info.mRootPath);
				EXPECT_EQ(info->mRelativePath, pa.info.mRelativePath);
				EXPECT_EQ(info->mParentPath, pa.info.mParentPath);
				EXPECT_EQ(info->mFileName, pa.info.mFileName);
				EXPECT_EQ(info->mStem, pa.info.mStem);
				EXPECT_EQ(info->mExtension, pa.info.mExtension);
			}
		}
	}
	catch (const std::exception & ex)
	{
		std::cout << ex.what() << std::endl;
		EXPECT_TRUE(false);
	}
	return true;
}