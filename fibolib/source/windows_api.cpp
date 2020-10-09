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
	bool validPathLength(size_t len) noexcept
	{
		return (len >= kMinAbPath) && (len < kMaxAbPath);
	}

	std::wstring absolutePath(std::wstring_view relativePath)
	{
		WCHAR wcAbsPath[kMaxAbPath] = { 0 };
		WCHAR** lppPart = { NULL };

		// Retrieve the full path name for a file. 
		// The file does not need to exist.
		auto retVal = ::GetFullPathNameW(relativePath.data(), kMaxAbPath, wcAbsPath, lppPart);

		if (0 == retVal)
		{
			throw std::exception(fmt::format("[{}:{}] Failed to convert relative path. Error: {}", 
				__FUNCTION__,
				__LINE__,
				::GetLastError()).c_str());
		}
		return std::wstring(wcAbsPath);
	}

	std::wstring canonicalize(std::wstring_view sPath)
	{
		WCHAR wcAbsPath[kMaxAbPath] = { 0 };
		auto retVal = ::PathCchCanonicalize(wcAbsPath, kMaxAbPath, sPath.data());
		if (FAILED(retVal))
		{
			throw std::exception(fmt::format("[{}:{}] Failed to convert canonical path. Error: {}",
				__FUNCTION__,
				__LINE__,
				::GetLastError()).c_str());
		}
		return std::wstring(wcAbsPath);
	}
}
#endif // _WIN32