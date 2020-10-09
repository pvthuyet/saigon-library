#pragma once

#include <string>

namespace fibo::StringConverter
{
	std::string wc2mb(const wchar_t* str, int len, unsigned int codePage = 65001); // CP_UTF8
	std::wstring mb2wc(const char* str, int len, unsigned int codePage = 65001); // CP_UTF8
}

