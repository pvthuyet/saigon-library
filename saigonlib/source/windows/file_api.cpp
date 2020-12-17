#ifdef USE_WINDOWS_API

#include "windows/file_api.h"
#include <Windows.h>
#include <pathcch.h>
#include <fmt/format.h>

#pragma comment(lib, "Pathcch.lib")
constexpr int kMinAbPath = 3;
constexpr int kMaxAbPath = MAX_PATH;

namespace saigon::winapi
{
	bool FileApi::validPathLength(size_t len) noexcept
	{
		return (len >= kMinAbPath) && (len < kMaxAbPath);
	}

	std::wstring FileApi::absolutePath(std::wstring_view relativePath)
	{
		WCHAR wcAbsPath[kMaxAbPath] = { 0 };
		WCHAR** lppPart = { NULL };

		// Retrieve the full path name for a file. 
		// The file does not need to exist.
		auto retVal = ::GetFullPathNameW(relativePath.data(), kMaxAbPath, wcAbsPath, lppPart);

		if (0 == retVal)
		{
			throw std::runtime_error(fmt::format("Failed to convert relative path. Error: {}. {}:{}", 
				::GetLastError(),
				__FILE__,
				__LINE__));
		}
		return std::wstring(wcAbsPath);
	}

	std::wstring FileApi::canonicalize(std::wstring_view sPath)
	{
		WCHAR wcAbsPath[kMaxAbPath] = { 0 };
		auto retVal = ::PathCchCanonicalize(wcAbsPath, kMaxAbPath, sPath.data());
		if (FAILED(retVal))
		{
			throw std::runtime_error(fmt::format("Failed to convert canonical path. Error: {}. {}:{}",
				::GetLastError(),
				__FILE__,
				__LINE__));
		}
		return std::wstring(wcAbsPath);
	}
}
#endif // USE_WINDOWS_API