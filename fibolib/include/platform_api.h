#pragma once

#include <string>
#include <filesystem>

namespace fibo::PlatformAPI
{
	bool validPathLength(size_t len);
#ifdef _WIN32
	_NODISCARD std::wstring absolutePath(std::wstring_view relativePath);
	_NODISCARD std::wstring canonicalize(std::wstring_view sPath);
#else

	template<typename T, typename = typename std::enable_if_t<
		std::is_same<std::string, typename std::decay_t<T>>::value
		|| std::is_same<std::wstring, typename std::decay_t<T>>::value
		>
	>
	_NODISCARD T canonicalize(const T& sPath)
	{
		namespace fs = std::filesystem;
		try
		{
			std::error_code ec;
			auto canPath = fs::canonical(sPath, ec);
			if (!ec)
			{
				if constexpr (std::is_same<std::wstring, typename std::decay_t<T>>::value)
				{
					return canPath.wstring();
				}
				else
				{
					return canPath.string();
				}
			}
		}
		catch(...)
		{ }
		return T{};
	}
#endif // _WIN32
}