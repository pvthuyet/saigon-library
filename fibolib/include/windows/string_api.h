#pragma once
#ifdef _WIN32

#include "define.h"
#include <string>

namespace fibo
{
	class StringUtils;
}

namespace fibo::WindowsApi
{
	class StringApi
	{
		friend class StringUtils;
		F_NODISCARD static std::string wc2mb(std::wstring_view str, unsigned int codePage = 65001); // 65001: CP_UTF8
		F_NODISCARD static std::wstring mb2wc(std::string_view str, unsigned int codePage = 65001); // 65001: CP_UTF8
	};
}
#endif // _WIN32