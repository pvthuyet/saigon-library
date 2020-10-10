#pragma once
#ifdef _WIN32

#include <string>

namespace fibo::WindowsApi
{
	class StringApi
	{
		_NODISCARD static std::string wc2mb(std::wstring_view str, unsigned int codePage = 65001); // 65001: CP_UTF8
		_NODISCARD static std::wstring mb2wc(std::string_view str, unsigned int codePage = 65001); // 65001: CP_UTF8
	};
}
#endif // _WIN32