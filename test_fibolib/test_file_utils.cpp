#include "pch.h"
#include "test_file_utils.h"
#include "file_utils.h"

using namespace fibo::FileUtils;

bool testParseFileName()
{
	std::string fpath = "C:\\tmp\\test.txt";
	std::string parent = "C:\\tmp";
	std::string root = "C:\\";
	std::string name = "test.txt";
	std::string stem = "test";
	std::string ext = ".txt";

	auto info = parseFileName(fpath);
	EXPECT_EQ(info.mFullPath, fpath);
	EXPECT_EQ(info.mParentPath, parent);
	EXPECT_EQ(info.mRootPath, root);
	EXPECT_EQ(info.mFileName, name);
	EXPECT_EQ(info.mStem, stem);
	EXPECT_EQ(info.mExtension, ext);
	return true;
}