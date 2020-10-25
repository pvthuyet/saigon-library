#pragma once

#ifdef _WIN32

#include "define.h"
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
		F_NODISCARD static bool validPathLength(size_t len) noexcept;
		F_NODISCARD static std::wstring absolutePath(std::wstring_view relativePath);
		F_NODISCARD static std::wstring canonicalize(std::wstring_view sPath);
	};
}

#endif