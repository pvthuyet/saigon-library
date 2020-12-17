#pragma once

#ifdef USE_WINDOWS_API

#include <string>

namespace saigon
{
	class PathUtils;
}

namespace saigon::winapi
{
	class FileApi
	{
		friend class PathUtils;
		[[nodiscard]] static bool validPathLength(size_t len) noexcept;
		[[nodiscard]] static std::wstring absolutePath(std::wstring_view relativePath);
		[[nodiscard]] static std::wstring canonicalize(std::wstring_view sPath);
	};
}

#endif // USE_WINDOWS_API