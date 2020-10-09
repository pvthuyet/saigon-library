#ifdef _WIN32

#include "windows_api.h"
#include <Windows.h>
#include <pathcch.h>

#pragma comment(lib, "Pathcch.lib")
constexpr int kMinAbPath = 3;
constexpr int kMaxAbPath = MAX_PATH;

namespace fibo::WindowsApi
{
	bool validPathLength(size_t len)
	{
		return (len >= kMinAbPath) && (len < kMaxAbPath);
	}

	_NODISCARD std::wstring absolutePath(std::wstring_view relativePath)
	{
		WCHAR wcAbPath[kMaxAbPath] = { 0 };
		WCHAR* lppPart = NULL;
		auto res = ::GetFullPathNameW(relativePath.data(), (int)relativePath.length(), wcAbPath, &lppPart);
		if (res > 0)
		{
			return std::wstring(wcAbPath);
		}
		return std::wstring{};
	}

	std::wstring canonicalize(std::wstring_view sPath)
	{
		WCHAR wcAbPath[kMaxAbPath] = { 0 };
		auto res = ::PathCchCanonicalize(wcAbPath, kMaxAbPath, sPath.data());
		if (SUCCEEDED(res))
		{
			return std::wstring(wcAbPath);
		}
		return std::wstring{};
	}
}
#endif // _WIN32