#pragma once

#ifdef _WIN32

#include <string>

namespace fibo::WindowsApi
{
	bool validPathLength(size_t len);
	_NODISCARD std::wstring absolutePath(std::wstring_view relativePath);
	_NODISCARD std::wstring canonicalize(std::wstring_view sPath);
}

#endif