#pragma once

#ifdef _WIN32

#include <string>

namespace fibo
{
	class PathUtils;
}

namespace fibo::WindowsApi
{
	class FileApi
	{
		friend class PathUtils;
		_NODISCARD static bool validPathLength(size_t len) noexcept;
		_NODISCARD static std::wstring absolutePath(std::wstring_view relativePath);
		_NODISCARD static std::wstring canonicalize(std::wstring_view sPath);
	};
}

#endif