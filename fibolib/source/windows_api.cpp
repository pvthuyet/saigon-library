#ifdef _WIN32

#include "windows_api.h"
#include <Windows.h>
#include <pathcch.h>
#include <fmt/core.h>

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
		WCHAR wcAbsPath[kMaxAbPath] = { 0 };
		WCHAR* lppPart = NULL;
		auto res = ::GetFullPathNameW(relativePath.data(), (int)relativePath.length(), wcAbsPath, &lppPart);

		if (0 == wcsnlen_s(wcAbsPath, kMaxAbPath)) {
			throw std::exception(fmt::format("Failed to convert relative path to absolute path by GetFullPathNameW. Error: {}", ::GetLastError()).c_str());
		}
		return std::wstring(wcAbsPath);
	}

	std::wstring canonicalize(std::wstring_view sPath)
	{
		WCHAR wcAbsPath[kMaxAbPath] = { 0 };
		auto res = ::PathCchCanonicalize(wcAbsPath, kMaxAbPath, sPath.data());
		if (SUCCEEDED(res))
		{
			return std::wstring(wcAbsPath);
		}
		return std::wstring{};
	}
}
#endif // _WIN32