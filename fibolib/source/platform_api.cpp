#include "platform_api.h"
#include "string_converter.h"

#ifdef _WIN32
#include <Windows.h>
#include <pathcch.h>
#else
//++ TODO
#endif // _WIN32


#ifdef _WIN32
#pragma comment(lib, "Pathcch.lib")
constexpr int kMinAbPath = 3;
constexpr int kMaxAbPath = MAX_PATH;
#else
constexpr int kMinAbPath = 1;
constexpr int kMaxAbPath = 260;
#endif // _WIN32

namespace fibo::PlatformAPI
{
	bool validPathLength(size_t len)
	{
		return (len >= kMinAbPath) && (len < kMaxAbPath);
	}

#ifdef _WIN32
	_NODISCARD std::wstring absolutePath(std::wstring_view relativePath)
	{
		WCHAR wcAbPath[kMaxAbPath] = { 0 };
		WCHAR* lppPart = NULL;
		auto res = ::GetFullPathNameW(relativePath.data(), relativePath.length(), wcAbPath, &lppPart);
		if (res > 0)
		{
			return std::wstring(wcAbPath);
		}
		return std::wstring{};
	}

	std::wstring canonicalize(std::wstring_view sPath)
	{
		WCHAR wcAbPath[kMaxAbPath] = { 0 };
		auto res = PathCchCanonicalize(wcAbPath, kMaxAbPath, sPath.data());
		if (SUCCEEDED(res))
		{
			return std::wstring(wcAbPath);
		}
		return std::wstring{};
	}
#endif // _WIN32
}